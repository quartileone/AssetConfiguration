#ifndef ASSETCONFIGURATION_H
#define ASSETCONFIGURATION_H


#include "Configurations/jsonconfiguration.h"

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

    QString description() const { return m_description; }
    int id() const { return m_id; }

// IConfiguration implementation
private:
    void Serialize(ConfigSerializer& ser) override;
    void Deserialize(ConfigSerializer& desr) override;

protected:
    int m_id;
    QString m_key;
    QString m_description;
    QString m_persistence_profile;
    QJsonObject m_vpn;
    QJsonObject m_lan;
};


class AssetConfigurationList
        : public IConfigurationList
{
public:
    AssetConfigurationList()
    {
    }

    AssetConfiguration* Item(int index) override
    {
        return static_cast<AssetConfiguration *>(m_list[index].data());
    }

private:
    virtual void Serialize(ConfigSerializer& ser) override;
    virtual void Deserialize(ConfigSerializer& desr) override;
};


class SiteConfiguration
        :public JsonConfiguration
{
public:
    SiteConfiguration()
        : m_description(QString::null)
        , m_timeZone(QString::null)
        , m_uri(QString::null)
        , m_oauth(QString::null)
        , m_qos(QString::null)
        , m_persistence_qos(QString::null)
    {
    }

    AssetConfigurationList& Assets() { return m_assets; }

    QString description() const { return m_description; }
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
};


class SiteConfigurationList
        : public IConfigurationList
{
public:
    SiteConfigurationList()
    {
    }

    SiteConfiguration* Item(int index) override
    {
        return static_cast<SiteConfiguration *>(m_list[index].data());
    }

private:
    virtual void Serialize(ConfigSerializer& ser) override;
    virtual void Deserialize(ConfigSerializer& desr) override;

};



#endif // ASSETCONFIGURATION_H
