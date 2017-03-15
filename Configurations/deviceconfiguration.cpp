#include "deviceconfiguration.h"


void DeviceConfiguration::Serialize(ConfigSerializer &ser)
{
    JsonConfiguration::Serialize(ser);

     ser.Serialize("id",              m_id)
        .Serialize("serialNumber",    m_serialNumber)
        .Serialize("deviceHub",       m_deviceHub)
        .Serialize("cluster",         m_cluster)
        .Serialize("urn",             m_urn)
        .Serialize("openvpnserver",   m_openvpnServer)
        .Serialize("uri",             m_uri);
}

void DeviceConfiguration::Deserialize(ConfigSerializer &desr)
{
    desr.Deserialize("id",              m_id)
        .Deserialize("serialNumber",    m_serialNumber)
        .Deserialize("deviceHub",       m_deviceHub)
        .Deserialize("cluster",         m_cluster)
        .Deserialize("urn",             m_urn)
        .Deserialize("openvpnserver",   m_openvpnServer)
        .Deserialize("uri",             m_uri);
}
