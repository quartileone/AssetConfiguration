#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H

#include <QProcess>
#include <memory>
#include "DataModel/iconfiguration.h"
#include "Configurations/assetconfiguration.h"
#include "Configurations/localassetconfiguration.h"
#include "Configurations/currentconfiguration.h"

class ConfigurationManager
{
public:
    ConfigurationManager()
        : m_configuration(new LocalAssetConfiguration())
        , m_isAssetConfigured(false)
    {
    }

    bool Initialize();

    TUPSites GetAvailableConfiguration(QString configFilePath);

    void UserDone(IConfigurationPtr configuration);

    bool IsAssetConfigured() const { return m_isAssetConfigured; }

    CurrentConfiguration& currentConfiguration() { return m_currentConfiguration; }

    QString mountPath() const;
    QString configUrl() const;
    QString workingPath() const;
    QString userPassword() const { return m_configuration->password(); }
    QString serialNumber() const { return m_configuration->serialNumber(); }

private:
    bool SaveConfiguration(IConfiguration& configuration, QString strAbsoluteFilePath = QString::null);

    QString ReadJsonConfig(QString strJsonFile);

private:
   std::unique_ptr<LocalAssetConfiguration> m_configuration;
   CurrentConfiguration m_currentConfiguration;
   bool m_isAssetConfigured;
};

#endif // CONFIGURATIONMANAGER_H
