#include <QFile>
#include <QDir>
#include <QTableWidget>

#include "mainwindow.h"
#include "Utilities/terminalcmdexecutor.h"
#include "Utilities/usbconfigmounter.h"

#include "Configurations/localassetconfiguration.h"
#include "Configurations/assetconfiguration.h"
#include "Configurations/deviceconfiguration.h"
#include "Configurations/currentconfiguration.h"

#include "MineqWidgets/assettablewidget.h"

#define START_CONFIG_FILE_NAME "StartDeviceConfiguration.sh"
#define SIDE_LOAD_DIR "/mineq/sideload/"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_usbWatcher(new QDeviceWatcher())
    , m_vmshareWatcher(new QFileSystemWatcher())
    , m_configManager(new ConfigurationManager())
    , m_tabManager (nullptr)
    , m_rebootOnUsbDetach(false)
{
    ui->setupUi(this);
    m_tabManager.reset(new MineqTabManager(ui->tabWidget, ui->edSearch, ui->OkButton));

    ui->centralWidget->setAttribute(Qt::WA_QuitOnClose);
    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
    ui->labelConfig->setAlignment(Qt::AlignRight | Qt::AlignCenter);
}

MainWindow::~MainWindow()
{
    m_usbWatcher->disconnect();
    m_vmshareWatcher->disconnect();
    delete ui;
}

bool MainWindow::Initialize()
{
    m_usbWatcher->appendEventReceiver(this);
    connect(m_usbWatcher.get()
            , SIGNAL(deviceAdded(QString))
            , this
            , SLOT(slot_usb_added(QString))
            , Qt::DirectConnection);

    connect(m_usbWatcher.get()
            , SIGNAL(deviceRemoved(QString))
            , this
            , SLOT(slot_usb_removed(QString))
            , Qt::DirectConnection);
    if (!m_usbWatcher->start()) {
        return false;
    }

    connect(m_vmshareWatcher.get(), SIGNAL(directoryChanged(const QString &)), this,
            SLOT(slot_side_load_config_event(const QString &)));
    QString sideLoadConfig = SIDE_LOAD_DIR;
    slot_side_load_config_event(sideLoadConfig);
    m_vmshareWatcher->addPath(sideLoadConfig);
    ui->edSearch->addAction(QIcon(":/images/search.svg"), QLineEdit::LeadingPosition);

    if (!m_configManager->Initialize()) {
        return false;
    }
    return true;
}

void MainWindow::Start()
{
    // if (!m_configManager->IsAssetConfigured())
    TerminalCmdExecutor::LockScreen();
    ShowDefaultView();
}

void MainWindow::ShowManualConfiguration(QDir mountedConfigDir)
{
    QString configFile = mountedConfigDir.absoluteFilePath(USB_CONFIG_FILE_NAME);
    TUPSites configList = m_configManager->GetAvailableConfiguration(configFile);
    if (!configList) {
        MineqMessageWidget* msgWidget = new MineqMessageWidget();
        msgWidget->CriticalMessage("Cannot read config file.\nThe file could be damaged or empty.\nPlease remove USB key create new file and try again.");
        m_tabManager->AddOneCentralWidget(msgWidget, "Error");
    } else {
        ui->edSearch->setVisible(true);
        ui->edSearch->setText("");
        ui->OkButton->setVisible(true);
        ui->cancelButton->setVisible(true);
        ui->sideloadButton->setVisible(false);
        m_tabManager->InitSitesConfig(std::move(configList));
        if (!m_tabManager->AddTableWidget()) {
            MineqMessageWidget* msgWidget = new MineqMessageWidget();
            msgWidget->InformationMessage("No assets in config.json. Please, select another one");
            m_tabManager->AddOneCentralWidget(msgWidget, "Config loader");
        }
    }
}

void MainWindow::ShowDefaultView()
{
    m_tabManager->DisableOKButton();

    ui->OkButton->setVisible(false);
    ui->cancelButton->setVisible(false);

    ui->edSearch->setVisible(false);
    ui->sideloadButton->setVisible(false);

    MineqMessageWidget* msgWidget = new MineqMessageWidget();
    msgWidget->DefaultMessage();
    m_tabManager->AddOneCentralWidget(msgWidget, "USB key");
}

void MainWindow::ReconfigurAsset(QString mountPath)
{
    MineqMessageWidget* msgWidget = new MineqMessageWidget();

    QString strMsg = "The current asset configuration is:\n";
    strMsg += "Site: ";
    strMsg += m_configManager->currentConfiguration().site();
    strMsg += '\n';
    strMsg += "Asset: ";
    strMsg += m_configManager->currentConfiguration().asset();
    strMsg += "\n\n";
    strMsg += "Do you want to reconfigure it?";
    msgWidget->QuestionMessage(mountPath, strMsg);
    m_tabManager->AddOneCentralWidget(msgWidget, "Information");

    connect(msgWidget
            , SIGNAL(mineqMesgClicked(QString, MineqButton))
            , this
            , SLOT(slot_on_mineq_msg_button_clicked(QString, MineqButton)));
}

void MainWindow::on_sideloadButton_clicked() {

    m_tabManager->ClearTabs();

    QDir configDir(SIDE_LOAD_DIR);

    if (m_configManager->IsAssetConfigured()) {
        ReconfigurAsset(SIDE_LOAD_DIR);
    } else {
        ShowManualConfiguration(configDir);
    }

    slot_side_load_config_event(SIDE_LOAD_DIR);
}

void MainWindow::on_OkButton_clicked()
{
    ui->OkButton->setVisible(false);
    ui->cancelButton->setVisible(false);

    AssetTableWidget* assetTable = dynamic_cast<AssetTableWidget*>(ui->tabWidget->currentWidget());
    AssetTableWidgetItem* tableItem = dynamic_cast<AssetTableWidgetItem*>
            (assetTable->item(assetTable->currentRow(), assetTable->currentColumn()));

    std::unique_ptr<JsonConfiguration> pocessConfig(new JsonConfiguration());
    pocessConfig->InsertValue("shFile", m_configManager->workingPath() + QDir::separator() + START_CONFIG_FILE_NAME);
    pocessConfig->InsertValue("url", m_configManager->configUrl());
    pocessConfig->InsertValue("workingPath", m_configManager->workingPath());

    AssetConfigProcessWidget* configProcWidget = new AssetConfigProcessWidget(pocessConfig);
    connect(configProcWidget
            , SIGNAL(configFinished(IConfigurationPtr, ConfigurationType))
            , this
            , SLOT(slot_configuration_finished(IConfigurationPtr, ConfigurationType)));

    JsonConfigurationPtr assetConfiguration(new JsonConfiguration());

    AssetConfigurationPtr asset = tableItem->configuration();

    SiteConfigurationPtr site = std::dynamic_pointer_cast<SiteConfiguration>(asset->getSite().lock());

    assetConfiguration->InsertConfiguration("", site);
    assetConfiguration->InsertConfiguration("assetConfigs", asset);

    m_tabManager->AddOneCentralWidget(configProcWidget, "Configuring");

    configProcWidget->StartConfiguration(m_mountedPath, assetConfiguration);
}

void MainWindow::on_cancelButton_clicked()
{
    TerminalCmdExecutor::LockScreen();
    ShowDefaultView();
}

void MainWindow::slot_side_load_config_event(const QString & path)
{
    QString config = path + USB_CONFIG_FILE_NAME;
    QFile sideloadConfig(config);
    if (sideloadConfig.exists()) {
        ui->sideloadButton->setVisible(true);
        TerminalCmdExecutor::UnlockScreen(m_configManager->userPassword());
    } else {
        ui->sideloadButton->setVisible(false);
        TerminalCmdExecutor::LockScreen();
    }
    sideloadConfig.close();
}

void MainWindow::slot_usb_added(const QString &dev)
{
    std::shared_ptr<UsbConfigMounter> configMounter(new UsbConfigMounter(QDir(dev)));

    configMounter->MountConfigUsbFlash(m_configManager->mountPath());
    if (configMounter->IsConfigUsbFlash()) {
        m_tabManager->ClearTabs();
        TerminalCmdExecutor::UnlockScreen(m_configManager->userPassword());

        m_mountedPath = configMounter->usbMountedDir().absolutePath();

        if (m_configManager->IsAssetConfigured()) {
            ReconfigurAsset(configMounter->usbMountedDir().absolutePath());
        } else {
            ShowManualConfiguration(configMounter->usbMountedDir());
        }
    } else {
        if (configMounter->IsMounteDirExist(m_configManager->mountPath())) {
            configMounter->UnmountConfigUsbFlash(m_configManager->mountPath());
        }
    }
}

void MainWindow::slot_usb_removed(const QString &dev)
{
    {
        std::shared_ptr<UsbConfigMounter> configMounter(new UsbConfigMounter(QDir(dev)));

        if (configMounter->IsMounteDirExist(m_configManager->mountPath())) {
            configMounter->UnmountConfigUsbFlash(m_configManager->mountPath());
        }
    }

    if(m_rebootOnUsbDetach) {
        TerminalCmdExecutor::Reboot();
    } else {
        TerminalCmdExecutor::LockScreen();
        ShowDefaultView();
    }
}

void MainWindow::slot_configuration_finished(IConfigurationPtr configuration, ConfigurationType /*configType*/)
{
    QString tabTitle;
    MineqMessageWidget* msgWidget = new MineqMessageWidget();

    if (configuration) {
        m_configManager->UserDone(configuration);

        msgWidget->InformationMessage();
        tabTitle = "Configured";

        m_rebootOnUsbDetach = true;
    } else {
        msgWidget->CriticalMessage();
        tabTitle = "Error";
    }
    m_tabManager->AddOneCentralWidget(msgWidget, std::move(tabTitle));
}

void MainWindow::slot_on_mineq_msg_button_clicked(QString val, MineqButton but)
{
    if (but == MineqButton::Yes) {
        m_tabManager->ClearTabs();
        ShowManualConfiguration(QDir(val));
    } else {
        TerminalCmdExecutor::LockScreen();
        ShowDefaultView();
    }
}

void MainWindow::on_edSearch_textChanged(const QString &)
{
    bool assetsExist = m_tabManager->AddTableWidget();

    if (ui->edSearch->text() != "" && !assetsExist) {
        MineqMessageWidget* msgWidget = new MineqMessageWidget();
        msgWidget->InformationMessage("No assets match the search criteria - remove or update criteria.");
        m_tabManager->AddOneCentralWidget(msgWidget, "Search results", false);
    }
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    m_tabManager->ChangeTabIndex(index);
}

