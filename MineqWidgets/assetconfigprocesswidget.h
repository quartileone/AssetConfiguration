#ifndef ASSETCONFIGPROCESSWIDGET_H
#define ASSETCONFIGPROCESSWIDGET_H

#include <QWidget>
#include <QProcess>
#include <QSharedPointer>

#include <QMovie>
#include <QLabel>

#include <QtNetwork/QNetworkReply>

#include "DataModel/configserializer.h"
#include "Configurations/jsonconfiguration.h"

enum class ConfigurationType
{
    Auto,
    Manual
};

class AssetConfigProcessWidget
        : public QWidget
{
    Q_OBJECT
public:
    explicit AssetConfigProcessWidget(std::unique_ptr<JsonConfiguration> &configuration, QWidget *parent = nullptr);
    ~AssetConfigProcessWidget();

    void StartConfiguration(QString & usbMountedPath, JsonConfigurationPtr &assetConfiguration);

private:
    void ApplyConfiguration(QString & usbMountedPath, JsonConfigurationPtr &assetConfiguration);

    void DownloadConfiguration();

signals:
    void configFinished(IConfigurationPtr configuration, ConfigurationType configType);

private slots:
    void slot_config_downloaded(QNetworkReply* reply);
    void slot_read_config_stdout();
    void slot_config_finished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QProcess* m_configProcess;
    QMovie* m_progress;
    JsonConfigurationPtr m_assetConfiguration;
    std::unique_ptr<JsonConfiguration> m_configuration;
    QLabel* m_logsLabel;
    ConfigurationType m_configurationType;
};

#endif // ASSETCONFIGPROCESSWIDGET_H
