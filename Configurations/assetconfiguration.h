#ifndef ASSETCONFIGURATION_H
#define ASSETCONFIGURATION_H


#include "Configurations/jsonconfiguration.h"
#include <functional>

class AssetConfiguration
        : public JsonConfiguration
{
public:
    AssetConfiguration()
        : m_id(0)
        , m_key(QString::null)
        , m_description(QString::null)
    {
    }

    QString key() const { return m_key; }
    int id() const { return m_id; }

    std::weak_ptr<JsonConfiguration> getSite() {
        return m_site;
    }
    void setSite(std::shared_ptr<JsonConfiguration> conf) {
        m_site = conf;
    }


// IConfiguration implementation
private:
    void Serialize(ConfigSerializer& ser) override;
    void Deserialize(ConfigSerializer& desr) override;

private:
    int m_id;
    QString m_key;
    QString m_description;
    QString m_persistence_profile;
    QJsonObject m_vpn;
    QJsonObject m_lan;
    std::weak_ptr<JsonConfiguration> m_site;
};

typedef std::shared_ptr<AssetConfiguration> AssetConfigurationPtr;

class AssetConfigurationList : public IConfigurationList
{
public:
    using TComparator = std::function<bool(const std::shared_ptr<IConfiguration> &, const std::shared_ptr<IConfiguration> &)>;
    AssetConfigurationList() {};

    void SortAssets();

    static TComparator GetComparator() {
        return m_comparator;
    }
private:
    virtual void Serialize(ConfigSerializer& ser) override;
    virtual void Deserialize(ConfigSerializer&) override;
    static TComparator m_comparator;
};


class SiteConfiguration: public JsonConfiguration
{
public:
    SiteConfiguration()
        : m_description(QString::null)
        , m_timeZone(QString::null)
        , m_uri(QString::null)
        , m_oauth(QString::null)
        , m_qos(QString::null)
        , m_persistence_qos(QString::null)
        , m_device_docker_compose(QString::null)
        , m_bandwidth_constraint(QString::null)
    {
    }

    AssetConfigurationList& Assets() { return m_assets; }

    QString description() const { return m_description; }
    void description(const QString &p_description) { m_description = p_description; }
    QString timeZone() const { return m_timeZone; }
    QJsonObject cluster() const { return m_cluster; }
    QJsonObject deviceHub() const { return m_deviceHub; }

private:
    virtual void Serialize(ConfigSerializer& ser) override;
    virtual void Deserialize(ConfigSerializer& desr) override;

protected:
    QString m_description;
    QString m_timeZone;
    QJsonValue m_ntpServer;
    QJsonObject m_cluster;
    QJsonObject m_deviceHub;
    AssetConfigurationList m_assets;
    QString m_uri;   // uri to config api
    QString m_oauth; // uri to oauth for config api
    QString m_qos; // qos for devices
    QString m_persistence_qos; // qos for persistence
    QString m_device_docker_compose; // YAML file for docker compose
    QString m_bandwidth_constraint; // bandwidth constraint for primary tun0 interface on OpenVPN container
};

typedef std::shared_ptr<SiteConfiguration> SiteConfigurationPtr;


class SiteConfigurationList
        : public IConfigurationList
{
public:
    SiteConfigurationList()
    {
    }

private:
    void SortSites();
    virtual void Serialize(ConfigSerializer& ser) override;
    virtual void Deserialize(ConfigSerializer& desr) override;

};

typedef std::unique_ptr<SiteConfigurationList> TUPSites;

#endif // ASSETCONFIGURATION_H
