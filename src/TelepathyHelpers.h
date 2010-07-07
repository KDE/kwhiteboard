#ifndef TELEPATHY_HELPERS_H
#define TELEPATHY_HELPERS_H

#include <QtDBus/QDBusArgument>

struct StreamTubeAddress {
    QString address;
    uint port;
};

Q_DECLARE_METATYPE(StreamTubeAddress);

QDBusArgument &operator<<(QDBusArgument &argument,
        const StreamTubeAddress &streamTubeAddress);
const QDBusArgument &operator>>(const QDBusArgument &argument,
        StreamTubeAddress &streamTubeAddress);


#endif // Header guard

