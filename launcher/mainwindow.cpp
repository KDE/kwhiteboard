/*
 * Main KWhiteboard/KBlackboard Launcher Window
 *
 * Copyright (C) 2011 David Edmundson <kde@davidedmundson.co.uk>
 * Copyright (C) 2012 Daniele E. Domenichelli <daniele.domenichelli@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <KApplication>
#include <KDebug>
#include <KMessageBox>
#include <KLineEdit>

#include <QtGui/QPushButton>

#include <TelepathyQt/AccountManager>
#include <TelepathyQt/PendingChannelRequest>
#include <TelepathyQt/PendingReady>

#include <KTp/contact-factory.h>
#include <KTp/Models/contacts-list-model.h>
#include <KTp/Models/contacts-filter-model.h>
#include <KTp/Widgets/contact-grid-widget.h>

//FIXME, copy and paste the approver code for loading this from a config file into this, the contact list and the chat handler.
#define PREFERRED_KWHITEBOARD_HANDLER "org.freedesktop.Telepathy.Client.KTp.KWhiteboard"
#define PREFERRED_KBLACKBOARD_HANDLER "org.freedesktop.Telepathy.Client.KTp.KBlackboard"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow),
    m_contactsListModel(0)
{
    Tp::registerTypes();

    ui->setupUi(this);
    setWindowTitle(i18n("Start KWhiteboard/KBlackboard Session"));

    kDebug() << KApplication::arguments();

    Tp::AccountFactoryPtr  accountFactory = Tp::AccountFactory::create(QDBusConnection::sessionBus(),
                                                                       Tp::Features() << Tp::Account::FeatureCore
                                                                       << Tp::Account::FeatureAvatar
                                                                       << Tp::Account::FeatureProtocolInfo
                                                                       << Tp::Account::FeatureProfile
                                                                       << Tp::Account::FeatureCapabilities);

    Tp::ConnectionFactoryPtr connectionFactory = Tp::ConnectionFactory::create(QDBusConnection::sessionBus(),
                                                                               Tp::Features() << Tp::Connection::FeatureCore
                                                                               << Tp::Connection::FeatureRosterGroups
                                                                               << Tp::Connection::FeatureRoster
                                                                               << Tp::Connection::FeatureSelfContact);

    Tp::ContactFactoryPtr contactFactory = KTp::ContactFactory::create(Tp::Features()  << Tp::Contact::FeatureAlias
                                                                      << Tp::Contact::FeatureAvatarData
                                                                      << Tp::Contact::FeatureSimplePresence
                                                                      << Tp::Contact::FeatureCapabilities);

    Tp::ChannelFactoryPtr channelFactory = Tp::ChannelFactory::create(QDBusConnection::sessionBus());

    m_accountManager = Tp::AccountManager::create(QDBusConnection::sessionBus(),
                                                  accountFactory,
                                                  connectionFactory,
                                                  channelFactory,
                                                  contactFactory);

    m_contactsListModel = new KTp::ContactsListModel(this);
    connect(m_accountManager->becomeReady(), SIGNAL(finished(Tp::PendingOperation*)), SLOT(onAccountManagerReady()));

    m_contactGridWidget = new KTp::ContactGridWidget(m_contactsListModel, this);
    m_contactGridWidget->contactFilterLineEdit()->setClickMessage(i18n("Search in Contacts..."));
    m_contactGridWidget->filter()->setPresenceTypeFilterFlags(KTp::ContactsFilterModel::ShowOnlyConnected);
    //m_contactGridWidget->filter()->setCapabilityFilterFlags(KTp::ContactsFilterModel::FilterByFileTransferCapability);
    ui->recipientVLayout->insertWidget(1, m_contactGridWidget);

    connect(m_contactGridWidget,
            SIGNAL(selectionChanged(Tp::AccountPtr,Tp::ContactPtr)),
            SLOT(onContactSelectionChanged(Tp::AccountPtr,Tp::ContactPtr)));

    ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);
    connect(ui->buttonBox, SIGNAL(accepted()), SLOT(onDialogAccepted()));
    connect(ui->buttonBox, SIGNAL(rejected()), SLOT(close()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onAccountManagerReady()
{
    m_contactsListModel->setAccountManager(m_accountManager);
}

void MainWindow::onDialogAccepted()
{
    // don't do anytghing if no contact has been selected
    if (!m_contactGridWidget->hasSelection()) {
        // show message box?
        return;
    }

    Tp::ContactPtr contact = m_contactGridWidget->selectedContact();
    Tp::AccountPtr account = m_contactGridWidget->selectedAccount();

    if (account.isNull()) {
        kDebug() << "Account: NULL";
    } else {
        kDebug() << "Account is: " << account->displayName();
        kDebug() << "Contact is: " << contact->alias();
    }

    Tp::PendingChannelRequest *channelRequest;
    if (ui->kwhiteboardRadioButton->isChecked()) {
        // start dbustube
        channelRequest = account->createDBusTube(contact,
                                                 QLatin1String("org.kde.KWhiteboard"),
                                                 QDateTime::currentDateTime(),
                                                 PREFERRED_KWHITEBOARD_HANDLER);
    } else {
        // start streamtube
        channelRequest = account->createStreamTube(contact,
                                                   QLatin1String("kblackboard"),
                                                   QDateTime::currentDateTime(),
                                                   PREFERRED_KBLACKBOARD_HANDLER);
    }

    connect(channelRequest, SIGNAL(finished(Tp::PendingOperation*)), SLOT(slotCreateTubeFinished(Tp::PendingOperation*)));
}

void MainWindow::onContactSelectionChanged(Tp::AccountPtr account, Tp::ContactPtr contact)
{
    Q_UNUSED(account)
    Q_UNUSED(contact)

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(m_contactGridWidget->hasSelection());
}

void MainWindow::slotCreateTubeFinished(Tp::PendingOperation* op)
{
    if (op->isError()) {
        //FIXME map to human readable strings.
        QString errorMsg(op->errorName() + ": " + op->errorMessage());
        kDebug() << "ERROR!: " << errorMsg;
        KMessageBox::error(this, i18n("Failed to initiate KWhiteboard/KBlackboard Session"), i18n("KWhiteboard/KBlackboard Session Failed"));
        close();
    } else {
        kDebug() << "KWhiteboard/KBlackboard Session Started";
        // now I can close the dialog
        close();
    }
}
