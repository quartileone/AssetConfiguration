#ifndef DEVICECONFIGURATION_H
#define DEVICECONFIGURATION_H

#include "Configurations/jsonconfiguration.h"

class DeviceConfiguration
        : public JsonConfiguration
{
public:
    DeviceConfiguration()
        : m_id(0)
        , m_serialNumber(QString::null)
        , m_uri(QString::null)
        , m_oauth(QString::null)
    {
    }

// IConfiguration implementation
private:
    void Serialize(ConfigSerializer& ser) override;
    void Deserialize(ConfigSerializer& desr) override;

private:
    int m_id;
    QString     m_serialNumber;
    QJsonObject m_deviceHub;
    QJsonObject m_cluster;
    QJsonObject m_urn;
    QJsonValue  m_openvpnServer;
    QString     m_uri;
    QString     m_oauth;

};

#endif // DEVICECONFIGURATION_H
