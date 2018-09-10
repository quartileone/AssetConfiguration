
#include <QJsonArray>

#include "assetconfiguration.h"


void AssetConfiguration::Serialize(ConfigSerializer &ser)
{
    ser.Serialize("id",                      m_id)
       .Serialize("key",                     m_key)
       .Serialize("description",             m_description)
       .Serialize("vpn",                     m_vpn)
       .Serialize("lan",                     m_lan)
       .Serialize("persistence_qos_profile", m_persistence_profile);
}

void AssetConfiguration::Deserialize(ConfigSerializer &desr)
{
    desr.Deserialize("id",                      m_id)
        .Deserialize("key",                     m_key)
        .Deserialize("description",             m_description)
        .Deserialize("vpn",                     m_vpn)
        .Deserialize("lan",                     m_lan)
        .Deserialize("persistence_qos_profile", m_persistence_profile);
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
    ser.Serialize("description",     m_description)
        .Serialize("cluster",         m_cluster)
        .Serialize("deviceHub",       m_deviceHub)
        .Serialize("timeZone",        m_timeZone)
        .Serialize("ntpServer",       m_ntpServer)
        .Serialize("oauth",           m_oauth)
        .Serialize("uri",             m_uri)
        .Serialize("qos",             m_qos)
        .Serialize("persistence_qos", m_persistence_qos)
        .Serialize("deviceDockerCompose", m_device_docker_compose)
        .Serialize("bandwidthConstraint", m_bandwidth_constraint);
}

void SiteConfiguration::Deserialize(ConfigSerializer &desr)
{
    desr.Deserialize("description",     m_description)
        .Deserialize("cluster",         m_cluster)
        .Deserialize("deviceHub",       m_deviceHub)
        .Deserialize("assetConfigs",    m_assets)
        .Deserialize("timeZone",        m_timeZone)
        .Deserialize("ntpServer",       m_ntpServer)
        .Deserialize("oauth",           m_oauth)
        .Deserialize("uri",             m_uri)
        .Deserialize("qos",             m_qos)
        .Deserialize("persistence_qos", m_persistence_qos)
        .Deserialize("deviceDockerCompose", m_device_docker_compose)
        .Deserialize("bandwidthConstraint", m_bandwidth_constraint);
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
