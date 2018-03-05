
#include <QVBoxLayout>

#include "DataModel/configserializer.h"
#include "Configurations/assetconfiguration.h"

#include "assetconfigprocesswidget.h"
#include <iostream>
#include <QFile>
#include <QDir>

#define QOS_DIR  "/mineq/device/qos"
#define QOS_FILE QOS_DIR "/USER_QOS_PROFILES.xml"

AssetConfigProcessWidget::AssetConfigProcessWidget(JsonConfiguration* configuration, QWidget *parent)
    : QWidget(parent)
    , m_configProcess(new QProcess())
    , m_progress(new QMovie())
    , m_configuration(configuration)
    , m_logsLabel(new QLabel())
{
    QString strLabelStyle ("QLabel"
                           "{"
                                "border: none;"
                                "qproperty-alignment: AlignCenter;"
                                "font: 18pt;"
                           "}");

    QString workingPath;
    m_configuration->TakeValue("workingPath", workingPath);
    m_progress->setFileName(workingPath + "/images/loader_mineq.gif");

    QVBoxLayout* vertLayout = new QVBoxLayout();
    vertLayout->setAlignment(Qt::AlignCenter);

    QLabel* progresLabel = new QLabel();
    progresLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    progresLabel->setStyleSheet(strLabelStyle);
    progresLabel->setMovie(m_progress);
    vertLayout->addWidget(progresLabel);

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
    delete m_configuration;

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

void AssetConfigProcessWidget::StartConfiguration(QString & usbMountedPath, IConfiguration *assetConfiguration)
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

void AssetConfigProcessWidget::ApplyQOS(JsonConfiguration *cfg)
{
    QString strQTQOS;
    cfg->TakeValue("qos", strQTQOS);

    std::string strQOS(strQTQOS.toLatin1().data());

    size_t pos;

    std::string strToFind("\\r\\n");

    while ((pos = strQOS.find(strToFind)) != std::string::npos) {
        auto it = strQOS.begin() + pos;
        strQOS.replace(it, it + strToFind.length(), "\r\n");
    }

    QDir dir(QOS_DIR);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QFile qosFile(QOS_FILE);

    if(qosFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qosFile.setPermissions(QFileDevice::ReadUser | QFileDevice::WriteUser |
                               QFileDevice::ReadGroup | QFileDevice::WriteGroup |
                               QFileDevice::ReadOther | QFileDevice::WriteOther );
        qosFile.write(strQOS.c_str());
        qosFile.close();
    }


}

void AssetConfigProcessWidget::ApplyConfiguration(QString & usbMountedPath, IConfiguration *assetConfiguration)
{
    m_assetConfiguration = QSharedPointer<IConfiguration>(assetConfiguration);

    JsonConfiguration *cfg = dynamic_cast<JsonConfiguration *>(assetConfiguration);
    ApplyQOS(cfg);

    QString shFile;
    m_configuration->TakeValue("shFile", shFile);

    QString data;
    ConfigSerializer::SerializeS(*assetConfiguration, data);

     m_configProcess->start(shFile, QStringList()
                << data << usbMountedPath);
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

        JsonConfiguration assetConfig;
        assetConfig.InsertConfiguration("", siteConfigList.Item(0));
        assetConfig.InsertConfiguration("", siteConfigList.Item(0)->Assets().Item(0));

        QString str;
        ApplyConfiguration(str, &assetConfig);
    } else {
        emit configFinished(NULL, m_configurationType);
    }
}

void AssetConfigProcessWidget::slot_read_config_stdout()
{
    m_logsLabel->setText(m_configProcess->readAllStandardOutput().data());
}

void AssetConfigProcessWidget::slot_config_finished(int /*exitCode*/, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::CrashExit) {
        emit configFinished(NULL, m_configurationType);
    }

    m_configProcess->close();

    emit configFinished(m_assetConfiguration.data(), m_configurationType);
}
