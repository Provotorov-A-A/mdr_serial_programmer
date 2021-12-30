#ifndef ABSTRACTHARDWAREINTERFACE_H
#define ABSTRACTHARDWAREINTERFACE_H

#include <QString>

template<typename SettingsType>
class AbstractHardwareInterface
{
public:
    AbstractHardwareInterface();
    virtual bool Configure(const SettingsType&);
    virtual ReadData(char *data, qint64 maxSize);
    virtual qint64 Write(const char *data, qint64 maxSize);
    QString InstanceName();
    QString InterfaceName();
};

#endif // ABSTRACTHARDWAREINTERFACE_H
