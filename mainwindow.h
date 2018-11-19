#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QString>
#include <QFileSystemWatcher>
#include "ui_mainwindow.h"
#include <qdevicewatcher.h>

#include "configurationmanager.h"
#include "mineqtabmanager.h"

#include "DataModel/iconfiguration.h"

#include "MineqWidgets/assetconfigprocesswidget.h"
#include "MineqWidgets/mineqmessagewidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool Initialize();

    void Start();

private:
    void ShowManualConfiguration(QDir mountedConfigDir);

    void ShowDefaultView();

    void ReconfigurAsset(QString mountPath);


private slots:
    void on_OkButton_clicked();

    void on_sideloadButton_clicked();

    void on_cancelButton_clicked();

    void slot_usb_added(const QString& dev);

    void slot_usb_removed(const QString& dev);

    void slot_configuration_finished(IConfigurationPtr configuration, ConfigurationType configType);

    void slot_on_mineq_msg_button_clicked(QString val, MineqButton but);

    void slot_side_load_config_event(const QString &path);

    void on_edSearch_textChanged(const QString &arg1);

private:
    Ui::MainWindow *                      ui;
    std::unique_ptr<QDeviceWatcher>       m_usbWatcher;
    std::unique_ptr<QFileSystemWatcher>   m_vmshareWatcher;
    std::unique_ptr<ConfigurationManager> m_configManager;
    std::unique_ptr<MineqTabManager>      m_tabManager;
    bool                                  m_rebootOnUsbDetach;
    QString                               m_mountedPath;
};

#endif // MAINWINDOW_H
