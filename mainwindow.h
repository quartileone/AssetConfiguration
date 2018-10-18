#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QString>
#include <QFileSystemWatcher>

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
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool Initialize();

    void Start();

private:
    void ShowManualConfiguration(QDir moundetConfigDir);

    void ShowDefaultView();

    void ReconfigurAsset(QString mountPath);


private slots:
    void on_OkButton_clicked();

    void on_sideloadButton_clicked();

    void on_cancelButton_clicked();

    void slot_on_table_cell_clicked(int row, int col);

    void slot_device_added(const QString& dev);

    void slot_device_removed(const QString& dev);

    void slot_configuration_finished(IConfiguration* configuration, ConfigurationType configType);

    void slot_on_mineq_msg_button_clicked(QString val, MineqButton but);

    void slot_side_load_config_event(const QString &path);
private:
    Ui::MainWindow          *ui;
    QDeviceWatcher          *m_usbWatcher;
    QFileSystemWatcher      *m_vmshareWatcher;
    ConfigurationManager    *m_configManager;
    MineqTabManager         *m_tabManager;
    bool m_rebootOnUsbDetach;
    QString m_mountedPath;
};

#endif // MAINWINDOW_H
