
#include <QVBoxLayout>

#include "DataModel/configserializer.h"
#include "Configurations/assetconfiguration.h"

#include "assetconfigprocesswidget.h"
#include <iostream>
#include <QFile>
#include <QDir>

#define DEVICE_ROOT "/mineq/device/"

AssetConfigProcessWidget::AssetConfigProcessWidget(std::unique_ptr<JsonConfiguration> &configuration, QWidget *parent)
    : QWidget(parent)
    , m_configProcess(new QProcess())
    , m_progress(new QMovie())
    , m_configuration(std::move(configuration))
    , m_logsLabel(new QLabel())
{
    QString strLabelStyle ("QLabel"
                           "{"
                                "border: none;"
                                "qproperty-alignment: AlignCenter;"
                                "font: 18pt;"
                           "}");

    m_progress->setFileName(":/images/loader_mineq.gif");

    QVBoxLayout* vertLayout = new QVBoxLayout();
    vertLayout->setAlignment(Qt::AlignCenter);

    QLabel* progressLabel = new QLabel();
    progressLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    progressLabel->setStyleSheet(strLabelStyle);
    progressLabel->setMovie(m_progress);
    vertLayout->addWidget(progressLabel);

    m_logsLabel->setStyleSheet(strLabelStyle);
    m_logsLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    m_logsLabel->setWordWrap(true);
    vertLayout->addWidget(m_logsLabel);

    this->setLayout(vertLayout);

    connect(m_configProcess
            , SIGNAL(readyReadStandardOutput())
            , this
            , SLOT(slot_read_config_stdout()));

    connect(m_configProcess
            , SIGNAL(finished(int,QProcess::ExitStatus))
            , this
            , SLOT(slot_config_finished(int, QProcess::ExitStatus)));
}

AssetConfigProcessWidget::~AssetConfigProcessWidget()
{
    m_configProcess->disconnect();
    delete m_configProcess;

    m_progress->stop();
    delete m_progress;

    QLayoutItem* item;
    while ( ( item = this->layout()->takeAt( 0 ) ) != NULL )
    {
        delete item->widget();
        delete item;
    }
    delete this->layout();
}

void AssetConfigProcessWidget::StartConfiguration(QString & usbMountedPath, JsonConfigurationPtr &assetConfiguration)
{
    m_progress->start();
    if (assetConfiguration) {
        m_configurationType = ConfigurationType::Manual;
        ApplyConfiguration(usbMountedPath, assetConfiguration);
    } else {
        m_configurationType = ConfigurationType::Manual;
        DownloadConfiguration();
    }
}

void AssetConfigProcessWidget::ApplyConfiguration(QString & usbMountedPath, JsonConfigurationPtr &assetConfiguration)
{
    m_assetConfiguration = assetConfiguration;

    QString shFile;
    m_configuration->TakeValue("shFile", shFile);

    QString data;
    ConfigSerializer::SerializeS(*assetConfiguration, data);

    m_configProcess->start(shFile, QStringList() << data << usbMountedPath);
}

void AssetConfigProcessWidget::DownloadConfiguration()
{
    m_logsLabel->setText("Downloading configuration...");
    QNetworkAccessManager* networkManager = new QNetworkAccessManager();
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slot_config_downloaded(QNetworkReply*)));

    QString strUrl;
    m_configuration->TakeValue("url", strUrl);
    QUrl url(strUrl);
    QNetworkRequest request(url);

    networkManager->get(request);
}

void AssetConfigProcessWidget::slot_config_downloaded(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QString strReplyContent = reply->readAll();
        SiteConfigurationList siteConfigList;
        ConfigSerializer::DeserializeS(siteConfigList, strReplyContent);

        JsonConfigurationPtr assetConfig(new JsonConfiguration());
        assetConfig->InsertConfiguration("", siteConfigList.Item<IConfiguration>(0));
        // why on the earth on 1st asset only???
        assetConfig->InsertConfiguration("", siteConfigList.Item<SiteConfiguration>(0)->Assets().Item<IConfiguration>(0));

        QString str;
        ApplyConfiguration(str, assetConfig);
    } else {
        emit configFinished(nullptr, m_configurationType);
    }
}

void AssetConfigProcessWidget::slot_read_config_stdout()
{
    m_logsLabel->setText(m_configProcess->readAllStandardOutput().data());
}

void AssetConfigProcessWidget::slot_config_finished(int /*exitCode*/, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::CrashExit) {
        emit configFinished(nullptr, m_configurationType);
    }

    m_configProcess->close();

    emit configFinished(m_assetConfiguration, m_configurationType);
}
