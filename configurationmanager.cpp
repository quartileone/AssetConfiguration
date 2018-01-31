
#include <QFile>
#include <QDir>
#include <QTextCodec>
#include <QJsonDocument>

#include "configurationmanager.h"

#include "Configurations/deviceconfiguration.h"
#include "Configurations/assetconfiguration.h"

#define LOCAL_CONFIG_FILE         "/mineq/usb-configuration/LocalConfig.json"
#define DEVICE_CONFIG_FILE_NAME   "deviceconfig.json"
#define CURRENT_CONFIG_FILE_NAME  "CurrentConfig.json"


bool ConfigurationManager::Initialize()
{
    QFile localConfigFile(LOCAL_CONFIG_FILE);
    if (localConfigFile.open(QIODevice::ReadOnly | QIODevice::Text) == false) {
        return false;
    }

    QString configData;
    configData = localConfigFile.readAll();
    localConfigFile.close();

    std::string fn = LOCAL_CONFIG_FILE;

    ConfigSerializer::DeserializeS(*m_configuration, configData);

    Directories dirs = m_configuration->directories();
    QFile currentConfigFile(dirs.rootDir() + dirs.workingDir() + QDir::separator() + CURRENT_CONFIG_FILE_NAME);
    if (currentConfigFile.exists()) {
        currentConfigFile.open(QIODevice::ReadOnly | QIODevice::Text);
        QString currentConfig = currentConfigFile.readAll();
        ConfigSerializer::DeserializeS(m_currentConfiguration, currentConfig);
    }
    currentConfigFile.close();

    QFile deviceConfigFile(dirs.rootDir() + dirs.workingDir() + QDir::separator() + CURRENT_CONFIG_FILE_NAME);
    m_isAssetConfigured = deviceConfigFile.exists();

    return true;
}

SiteConfigurationList* ConfigurationManager::GetAvailableConfiguration(QString configFilePath)
{
    SiteConfigurationList* configList = new SiteConfigurationList();
    QString strJsonData = ReadJsonConfig(configFilePath);
    if (strJsonData.isEmpty()) {
        return configList;
    }

    ConfigSerializer::DeserializeS(*configList, strJsonData);

    return configList;
}

void ConfigurationManager::UserDone(IConfiguration* configuration)
{
    DeviceConfiguration deviceConfig;
    // firstly deserialize local config
    ConfigSerializer::DeserializeC(deviceConfig, *m_configuration);
    // now take some overrides from asset config:
    ConfigSerializer::DeserializeC(deviceConfig, *configuration);

    Directories dirs = m_configuration->directories();
    QString path = dirs.rootDir() + dirs.deviceDir() + QDir::separator() + DEVICE_CONFIG_FILE_NAME;

    if (!SaveConfiguration(deviceConfig, path))
        qDebug() << "cannot save config " << path;

    JsonConfiguration siteConfig;
    siteConfig.InsertConfiguration("", configuration);
    QString siteDescription;
    siteConfig.TakeValue("description", siteDescription);
    m_currentConfiguration.setSite(siteDescription);

    AssetConfiguration assetConfig;
    siteConfig.TakeValue("assetConfigs", &assetConfig);
    m_currentConfiguration.setId(assetConfig.id());
    m_currentConfiguration.setAsset(assetConfig.description());

    SaveConfiguration(m_currentConfiguration);

    m_isAssetConfigured = true;
}


QString ConfigurationManager::mountPath() const
{
    Directories dirs = m_configuration->directories();

    QString mountPath = dirs.rootDir() + dirs.mountDir();

    return mountPath;
}

QString ConfigurationManager::configUrl() const
{
    Urn urn = m_configuration->urn();

    QString requestUrl = m_configuration->uri() + urn.asset() + m_configuration->serialNumber();

    return requestUrl;
}

QString ConfigurationManager::workingPath() const
{
    Directories dirs = m_configuration->directories();

    QString workingDir = dirs.rootDir() + dirs.workingDir();

    return workingDir;
}

bool ConfigurationManager::SaveConfiguration(IConfiguration& configuration, QString strAbsoluteFilePath)
{
    if(strAbsoluteFilePath.isEmpty()) {
        Directories dirs = m_configuration->directories();
        strAbsoluteFilePath = dirs.rootDir() + dirs.workingDir() + QDir::separator() + CURRENT_CONFIG_FILE_NAME;
    }

    QString strData;
    ConfigSerializer::SerializeS(configuration, strData);
    QFile configFile(strAbsoluteFilePath);
    if (configFile.open(QIODevice::WriteOnly | QIODevice::Text) == false) {
        return false;
    }

    configFile.write(strData.toLatin1().data(), strData.size());
    configFile.close();

    return true;
}

QString ConfigurationManager::ReadJsonConfig(QString strJsonFile)
{
    QString fileContent;
    QFile jsonFile( QFile::decodeName(strJsonFile.toLatin1()) );
    if (jsonFile.open(QIODevice::ReadOnly | QIODevice::Text) == false) {
        return QString::null;
    }

    QTextStream out(&jsonFile);
    QTextCodec * codec = QTextCodec::codecForName("UTF-8");
    fileContent = codec->toUnicode(out.readAll().toLatin1());

    jsonFile.close();

    return fileContent;
}
