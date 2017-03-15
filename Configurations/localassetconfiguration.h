#ifndef LOCALASSETCONFIGURATION_H
#define LOCALASSETCONFIGURATION_H

#include "Configurations/jsonconfiguration.h"

class Urn
        :public IConfiguration
{
public:
    Urn()
        : m_clusterManager(QString::null)
        , m_device(QString::null)
        , m_asset(QString::null)
    {
    }

    QString clusterManager() const { return m_clusterManager; }
    QString device() const { return m_device; }
    QString asset() const { return m_asset; }

// IConfiguration implementation
private:
    void Serialize(ConfigSerializer&);
    void Deserialize(ConfigSerializer& desr);


private:
    QString m_clusterManager;
    QString m_device;
    QString m_asset;
};


class Directories
        : public IConfiguration
{
public:
    Directories()
        : m_rootDir(QString::null)
        , m_workingDir(QString::null)
        , m_deviceDir(QString::null)
        , m_mountDir(QString::null)
    {
    }

    QString rootDir() const { return m_rootDir; }
    QString workingDir() const { return m_workingDir; }
    QString deviceDir() const { return m_deviceDir; }
    QString mountDir() const { return m_mountDir; }

// IConfiguration implementation
private:
    void Serialize(ConfigSerializer& ser);
    void Deserialize(ConfigSerializer& desr);

private:
    QString m_rootDir;
    QString m_workingDir;
    QString m_deviceDir;
    QString m_mountDir;
};


class LocalAssetConfiguration
        : public JsonConfiguration
{
public:
    LocalAssetConfiguration()
        : m_username(QString::null)
        , m_password(QString::null)
        , m_serialNumber(QString::null)
        , m_uri(QString::null)
    {
    }

    QString uri() const { return m_uri; }
    QString password() const { return m_password; }
    QString username() const { return m_username; }

    QString serialNumber() const { return m_serialNumber; }
    void setSerialNumber(const QString &serialNumber) { m_serialNumber = serialNumber; }

    Directories& directories() { return m_directories; }
    Urn& urn() { return m_urn; }

// IConfiguration implementation
private:
    void Serialize(ConfigSerializer& ser) override;
    void Deserialize(ConfigSerializer& desr) override;

private:
    QString m_username;
    QString m_password;
    QString m_serialNumber;
    QString m_uri;

    Directories m_directories;
    Urn m_urn;

    QJsonValue m_openvpnServer;
};


#endif // LOCALASSETCONFIGURATION_H
