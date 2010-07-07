#ifndef OUTGOING_TUBE_H
#define OUTGOING_TUBE_H

#include "TelepathyHelpers.h"

#include <QtCore/QObject>

#include <TelepathyQt4/Channel>

class OutgoingTube : public QObject
{
    Q_OBJECT
    
public:
    OutgoingTube(const Tp::ChannelPtr &channel, QObject *parent = 0);
    ~OutgoingTube();
    
private:
    Tp::ChannelPtr m_channel;
};


#endif // Header guard

