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
    explicit AssetConfigProcessWidget(JsonConfiguration* configuration, QWidget *parent = 0);
    ~AssetConfigProcessWidget();

    void StartConfiguration(QString & usbMountedPath, IConfiguration* assetConfiguration = NULL);

private:
    void ApplyQOSs(JsonConfiguration *cfg);
    void ApplyOneNodeToFile(JsonConfiguration *cfg, const QString &strNodeName, QString strFileName);
    void ApplyConfiguration(QString & usbMountedPath, IConfiguration* assetConfiguration);

    void DownloadConfiguration();

signals:
    void configFinished(IConfiguration* configuration, ConfigurationType configType);

private slots:
    void slot_config_downloaded(QNetworkReply* reply);
    void slot_read_config_stdout();
    void slot_config_finished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QProcess* m_configProcess;
    QMovie* m_progress;
    QSharedPointer<IConfiguration> m_assetConfiguration;
    JsonConfiguration* m_configuration;
    QLabel* m_logsLabel;
    ConfigurationType m_configurationType;
};

#endif // ASSETCONFIGPROCESSWIDGET_H
