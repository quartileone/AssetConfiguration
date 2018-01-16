
#include <QJsonArray>

#include "assetconfiguration.h"


void AssetConfiguration::Serialize(ConfigSerializer &ser)
{
    ser.Serialize("id",             m_id)
       .Serialize("key",            m_key)
       .Serialize("description",    m_description)
       .Serialize("vpn",            m_vpn)
       .Serialize("lan",            m_lan);
}

void AssetConfiguration::Deserialize(ConfigSerializer &desr)
{
    desr.Deserialize("id",          m_id)
        .Deserialize("key",         m_key)
        .Deserialize("description", m_description)
        .Deserialize("vpn",         m_vpn)
        .Deserialize("lan",         m_lan);
}

void AssetConfigurationList::Serialize(ConfigSerializer &/*ser*/)
{

}

void AssetConfigurationList::Deserialize(ConfigSerializer &desr)
{
    IConfiguration* siteConfig = new AssetConfiguration();
    siteConfig->Deserialize(desr);
    this->Add(siteConfig);
}


void SiteConfiguration::Serialize(ConfigSerializer &ser)
{
    ser.Serialize("description",    m_description)
       .Serialize("cluster",        m_cluster)
       .Serialize("deviceHub",      m_deviceHub)
       .Serialize("timeZone",       m_timeZone)
       .Serialize("ntpServer",      m_ntpServer)
       .Serialize("uri",            m_uri);
}

void SiteConfiguration::Deserialize(ConfigSerializer &desr)
{
    desr.Deserialize("description",     m_description)
        .Deserialize("cluster",         m_cluster)
        .Deserialize("deviceHub",       m_deviceHub)
        .Deserialize("assetConfigs",    m_assets)
        .Deserialize("timeZone",        m_timeZone)
        .Deserialize("ntpServer",       m_ntpServer)
        .Deserialize("uri",             m_uri);
}


void SiteConfigurationList::Serialize(ConfigSerializer &/*ser*/)
{

}

void SiteConfigurationList::Deserialize(ConfigSerializer &desr)
{
    QJsonArray siteArr = desr.root().value("siteConfigs").toArray();

    foreach (QJsonValue val, siteArr) {
        ConfigSerializer deserializer;
        deserializer.setRoot(val.toObject());
        IConfiguration* siteConfig = new SiteConfiguration();
        siteConfig->Deserialize(deserializer);
        this->Add(siteConfig);
    }
}
