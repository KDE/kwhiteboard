#include "TelepathyHelpers.h"

//Marshall the StreamTubeAddress data into a D-Bus argument
QDBusArgument &operator<<(QDBusArgument &argument,
        const StreamTubeAddress &streamTubeAddress)
{
    argument.beginStructure();
    argument << streamTubeAddress.address << streamTubeAddress.port;
    argument.endStructure();
    return argument;
}

// Retrieve the StreamTubeAddress data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument,
        StreamTubeAddress &streamTubeAddress)
{
    argument.beginStructure();
    argument >> streamTubeAddress.address >> streamTubeAddress.port;
    argument.endStructure();
    return argument;
}
