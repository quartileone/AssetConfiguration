#include <QFile>
#include <QDebug>
#include <QDir>
#include <QTableWidget>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Utilities/terminalcmdexecutor.h"
#include "Utilities/usbconfigmounter.h"

#include "Configurations/localassetconfiguration.h"
#include "Configurations/assetconfiguration.h"
#include "Configurations/deviceconfiguration.h"
#include "Configurations/currentconfiguration.h"

#include "MineqWidgets/assettablewidget.h"


#define START_CONFIG_FILE_NAME "StartDeviceConfiguration.sh"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_usbWatcher(new QDeviceWatcher())
    , m_configManager(new ConfigurationManager())
    , m_rebootOnUsbDetach(false)
{
    ui->setupUi(this);

    ui->centralWidget->setAttribute(Qt::WA_QuitOnClose);
    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
    ui->labelConfig->setAlignment(Qt::AlignRight | Qt::AlignCenter);

    ui->OkButton->setEnabled(false);
    ui->OkButton->setVisible(false);
    ui->cancelButton->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_configManager;

    m_usbWatcher->disconnect();
    delete m_usbWatcher;

    delete m_tabManager;
}

bool MainWindow::Initialize()
{
    m_tabManager = new MineqTabManager(ui->tabWidget);

    m_usbWatcher->appendEventReceiver(this);
    connect(m_usbWatcher
            , SIGNAL(deviceAdded(QString))
            , this
            , SLOT(slot_device_added(QString))
            , Qt::DirectConnection);

    connect(m_usbWatcher
            , SIGNAL(deviceRemoved(QString))
            , this
            , SLOT(slot_device_removed(QString))
            , Qt::DirectConnection);
    if (!m_usbWatcher->start()) {
        return false;
    }

    if (!m_configManager->Initialize()) {
        return false;
    }

    return true;
}

void MainWindow::Start()
{
    if (!m_configManager->IsAssetConfigured()) {

//        JsonConfiguration* config = new JsonConfiguration();
//        config->InsertValue("shFile", m_configManager->workingPath() + QDir::separator() + START_CONFIG_FILE_NAME);
//        config->InsertValue("url", m_configManager->configUrl());

//        AssetConfigProcessWidget* configProcWidget = new AssetConfigProcessWidget(config);
//        connect(configProcWidget
//                , SIGNAL(configFinished(IConfiguration*, ConfigurationType))
//                , this
//                , SLOT(slot_configuration_finished(IConfiguration*, ConfigurationType)));

//        m_tabManager->SetTabCount(1);
//        m_tabManager->AddMineqWidget(configProcWidget, "Configuring");

//        configProcWidget->StartConfiguration();
        TerminalCmdExecutor::LockScreen();
        ShowDefaultView();
    }
    else {
        TerminalCmdExecutor::LockScreen();
        ShowDefaultView();
    }
}


void MainWindow::ShowManualConfiguration(QDir moundetConfigDir)
{
    QString configFile = moundetConfigDir.absoluteFilePath(USB_CONFIG_FILE_NAME);
    SiteConfigurationList* configList = m_configManager->GetAvailableConfiguration(configFile);
    if (!configList) {
        m_tabManager->SetTabCount(1);
        MineqMessageWidget* msgWidget = new MineqMessageWidget(m_configManager->workingPath());
        msgWidget->CriticalMessage("Cannot read config file.\nThe file could be damaged or empty.\nPlease remove USB key create new file and try again.");
        m_tabManager->AddMineqWidget(msgWidget, "Error");
        return;
    }

    ui->OkButton->setVisible(true);
    ui->cancelButton->setVisible(true);
    m_tabManager->SetTabCount(configList->Size());

    int iRow = 0;
    for (int i = 0; i < configList->Size(); ++i) {
        AssetTableWidget* assetTable = new AssetTableWidget();
        assetTable->Initialize(configList->Item(i));
        connect(assetTable
                , SIGNAL(cellClicked(int, int))
                , this
                , SLOT(slot_on_table_cell_clicked(int,int)));

        m_tabManager->AddMineqWidget(assetTable, configList->Item(i)->description());
        ++iRow;
    }
}

void MainWindow::ShowDefaultView()
{
    ui->OkButton->setStyleSheet("border: none; color: #FFFFFF; background-color: rgba(106, 158, 236, 50); margin-right: 40px;");
    ui->OkButton->setEnabled(false);

    ui->OkButton->setVisible(false);
    ui->cancelButton->setVisible(false);

    m_tabManager->SetTabCount(1);
    MineqMessageWidget* msgWidget = new MineqMessageWidget(m_configManager->workingPath());
    msgWidget->DefaultMessage();
    m_tabManager->AddMineqWidget(msgWidget, "USB key");
}

void MainWindow::ReconfigurAsset(QString mountPath)
{
    m_tabManager->SetTabCount(1);
    MineqMessageWidget* msgWidget = new MineqMessageWidget(m_configManager->workingPath());

    QString strMsg = "The current asset configuration is:\n";
    strMsg += "Site: ";
    strMsg += m_configManager->currentConfiguration().site();
    strMsg += '\n';
    strMsg += "Asset: ";
    strMsg += m_configManager->currentConfiguration().asset();
    strMsg += "\n\n";
    strMsg += "Do you want to reconfigure it?";
    msgWidget->QuestionMessage(mountPath, strMsg);
    m_tabManager->AddMineqWidget(msgWidget, "Information");

    connect(msgWidget
            , SIGNAL(mineqMesgClicked(QString, MineqButton))
            , this
            , SLOT(slot_on_mineq_msg_button_clicked(QString, MineqButton)));
}


void MainWindow::on_OkButton_clicked()
{
    ui->OkButton->setVisible(false);
    ui->cancelButton->setVisible(false);

    AssetTableWidget* assetTable = dynamic_cast<AssetTableWidget*>(ui->tabWidget->currentWidget());
    AssetTableWidgetItem* tableItem = dynamic_cast<AssetTableWidgetItem*>(assetTable->item(assetTable->currentRow(), assetTable->currentColumn()));

    JsonConfiguration* pocessConfig = new JsonConfiguration();
    pocessConfig->InsertValue("shFile", m_configManager->workingPath() + QDir::separator() + START_CONFIG_FILE_NAME);
    pocessConfig->InsertValue("url", m_configManager->configUrl());
    pocessConfig->InsertValue("workingPath", m_configManager->workingPath());

    AssetConfigProcessWidget* configProcWidget = new AssetConfigProcessWidget(pocessConfig);
    connect(configProcWidget
            , SIGNAL(configFinished(IConfiguration*, ConfigurationType))
            , this
            , SLOT(slot_configuration_finished(IConfiguration*, ConfigurationType)));

    JsonConfiguration* assetConfiguration = new JsonConfiguration();

    assetConfiguration->InsertConfiguration("", assetTable->configuration());
    assetConfiguration->InsertConfiguration("assetConfigs", tableItem->configuration());

    m_tabManager->ClearTabs();

    m_tabManager->SetTabCount(1);
    m_tabManager->AddMineqWidget(configProcWidget, "Configuring");

    configProcWidget->StartConfiguration(m_mountedPath, assetConfiguration);
}

void MainWindow::on_cancelButton_clicked()
{
    TerminalCmdExecutor::LockScreen();

    ui->OkButton->setStyleSheet("border: none; color: #FFFFFF; background-color: rgba(106, 158, 236, 50); margin-right: 40px;");
    ui->OkButton->setEnabled(false);

    m_tabManager->ClearTabs();
    ShowDefaultView();
}

void MainWindow::slot_on_table_cell_clicked(int /*row*/, int /*col*/)
{
    QPushButton *but = ui->centralWidget->findChild<QPushButton*>("OkButton");

    QTableWidget* tableWidget = (QTableWidget*)ui->tabWidget->currentWidget();
    QTableWidgetItem* tableItem = tableWidget->item(tableWidget->currentRow(), tableWidget->currentColumn());

    if (tableItem) {
        but->setStyleSheet("border: none; color: #FFFFFF; background-color: rgb(106, 158, 236); margin-right: 40px");
        but->setEnabled(true);
    }
}

void MainWindow::slot_device_added(const QString &dev)
{
    UsbConfigMounter* configMounter = new UsbConfigMounter(QDir(dev));
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

    delete configMounter;
}

void MainWindow::slot_device_removed(const QString &dev)
{
    UsbConfigMounter* configMounter = new UsbConfigMounter(QDir(dev));
    if (configMounter->IsMounteDirExist(m_configManager->mountPath())) {
        configMounter->UnmountConfigUsbFlash(m_configManager->mountPath());
    }

    delete configMounter;

    if(m_rebootOnUsbDetach) {
        TerminalCmdExecutor::Reboot();
    }
    else {
        TerminalCmdExecutor::LockScreen();

        ui->OkButton->setStyleSheet("border: none; color: #B4CEF5; margin-right: 40px;");
        ui->OkButton->setEnabled(false);

        m_tabManager->ClearTabs();
        ShowDefaultView();
    }
}

void MainWindow::slot_configuration_finished(IConfiguration* configuration, ConfigurationType /*configType*/)
{
    m_tabManager->SetTabCount(1);
    MineqMessageWidget* msgWidget = new MineqMessageWidget(m_configManager->workingPath());

    if (configuration) {
        m_configManager->UserDone(configuration);
        m_tabManager->ClearTabs();

        msgWidget->InformationMessage();
        m_tabManager->AddMineqWidget(msgWidget, "Configured");

        m_rebootOnUsbDetach = true;
    } else {
        m_tabManager->ClearTabs();
        msgWidget->CriticalMessage();
        m_tabManager->AddMineqWidget(msgWidget, "Error");
    }
}

void MainWindow::slot_on_mineq_msg_button_clicked(QString val, MineqButton but)
{
    if(but == MineqButton::Yes) {
        m_tabManager->ClearTabs();
        ShowManualConfiguration(QDir(val));
    }
    else
    {
        TerminalCmdExecutor::LockScreen();
        m_tabManager->ClearTabs();
        ShowDefaultView();
    }
}
