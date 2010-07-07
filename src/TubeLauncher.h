#ifndef TUBE_LAUNCHER_H
#define TUBE_LAUNCHER_H

#include <QtCore/QObject>

#include <TelepathyQt4/AccountManager>
#include <TelepathyQt4/Account>
#include <TelepathyQt4/Connection>
#include <TelepathyQt4/Contact>
#include <TelepathyQt4/ContactManager>

namespace Tp {
    class PendingOperation;
}

class TubeLauncher : public QObject {

    Q_OBJECT

public:
    explicit TubeLauncher(QObject *parent = 0);
    ~TubeLauncher();

private Q_SLOTS:
    void onStartWhiteBoardSessionClicked();

    void onAccountManagerReady(Tp::PendingOperation *op);
    void onAccountReady(Tp::PendingOperation *op);
    void onConnectionReady(Tp::PendingOperation *op);
    void onContactsGot(Tp::PendingOperation *op);
    void onChannelReady(Tp::PendingOperation *op);

public Q_SLOTS:
    void onGotTubeChannel(Tp::ChannelPtr channel);

private:
    Tp::AccountManagerPtr m_accountManager;
    Tp::AccountPtr m_account;
    Tp::ConnectionPtr m_connection;
    QList<Tp::ContactPtr> m_targetContacts;
    Tp::ChannelPtr m_channel;

};

#endif

