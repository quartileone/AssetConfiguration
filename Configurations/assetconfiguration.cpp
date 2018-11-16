#include <QJsonArray>
#include <memory>
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

void AssetConfigurationList::SortAssets() {
    std::sort(m_list.begin(), m_list.end(), [this](const std::shared_ptr<IConfiguration> &lhs,
                                                   const std::shared_ptr<IConfiguration> &rhs) {
        const QString &lhsd = dynamic_cast<AssetConfiguration *>(lhs.get())->key();
        const QString &rhsd = dynamic_cast<AssetConfiguration *>(rhs.get())->key();

        return QString::compare(lhsd, rhsd, Qt::CaseInsensitive) < 0;
    });
}

void AssetConfigurationList::Deserialize(ConfigSerializer &desr)
{
    // logically it should be the same as in SiteConfigurationList but code is written in another way...
    std::shared_ptr<IConfiguration> assetConfig(new AssetConfiguration());
    assetConfig->Deserialize(desr);
    this->Add(assetConfig);
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
    // instead of sorting right inside AssetConfigurationList we should do that here:
    m_assets.SortAssets();
}


void SiteConfigurationList::Serialize(ConfigSerializer &/*ser*/)
{

}

void SiteConfigurationList::SortSites() {
    std::sort(m_list.begin(), m_list.end(), [this](const std::shared_ptr<IConfiguration> &lhs,
                                                   const std::shared_ptr<IConfiguration> &rhs) {
        const QString &lhsd = dynamic_cast<SiteConfiguration *>(lhs.get())->description();
        const QString &rhsd = dynamic_cast<SiteConfiguration *>(rhs.get())->description();

        return QString::compare(lhsd, rhsd, Qt::CaseInsensitive) < 0;
    });
}

void SiteConfigurationList::Deserialize(ConfigSerializer &desr)
{
    QJsonArray siteArr = desr.root().value("siteConfigs").toArray();

    CLear();
    foreach (QJsonValue val, siteArr) {
        ConfigSerializer deserializer;
        deserializer.setRoot(val.toObject());
        std::shared_ptr<IConfiguration> siteConfig(new SiteConfiguration());
        siteConfig->Deserialize(deserializer);
        Add(siteConfig);
    }
    SortSites();
}
