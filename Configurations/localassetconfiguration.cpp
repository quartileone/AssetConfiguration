
#include <QDebug>
#include <QString>

#include "localassetconfiguration.h"


void Urn::Serialize(ConfigSerializer &ser)
{
    ser.Serialize("clustermanager", m_clusterManager)
       .Serialize("device", m_device)
       .Serialize("asset", m_asset);
}

void Urn::Deserialize(ConfigSerializer &desr)
{
    desr.Deserialize("clustermanager",  m_clusterManager)
        .Deserialize("device",          m_device)
        .Deserialize("asset",           m_asset);
}


void Directories::Serialize(ConfigSerializer &/*ser*/)
{

}

void Directories::Deserialize(ConfigSerializer &desr)
{
    desr.Deserialize("root",    m_rootDir)
        .Deserialize("work",    m_workingDir)
        .Deserialize("device",  m_deviceDir)
        .Deserialize("mount",   m_mountDir);
}


void LocalAssetConfiguration::Serialize(ConfigSerializer &ser)
{
    JsonConfiguration::Serialize(ser);

     ser.Serialize("usename",        m_username)
        .Serialize("password",       m_password)
        .Serialize("serialNumber",   m_serialNumber)
        .Serialize("uri",            m_uri)
        .Serialize("directories",    &m_directories)
        .Serialize("urn",            &m_urn)
        .Serialize("openvpnserver",  m_openvpnServer);
}

void LocalAssetConfiguration::Deserialize(ConfigSerializer &desr)
{
    desr.Deserialize("usename",        m_username)
        .Deserialize("password",       m_password)
        .Deserialize("serialNumber",   m_serialNumber)
        .Deserialize("uri",            m_uri)
        .Deserialize("directories",    m_directories)
        .Deserialize("urn",            m_urn)
        .Deserialize("openvpnserver",  m_openvpnServer);
}
