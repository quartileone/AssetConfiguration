#ifndef ASSETTABLEWIDGET_H
#define ASSETTABLEWIDGET_H

#include <QTableWidget>
#include <memory>
#include <QSharedPointer>
#include <QPushButton>

#include "Configurations/assetconfiguration.h"
#include "Configurations/jsonconfiguration.h"

class AssetTableWidget : public QTableWidget
{
    Q_OBJECT

public:
    AssetTableWidget(QWidget *parent = nullptr);
    AssetTableWidget(const AssetTableWidget &) = delete;
    AssetTableWidget& operator = (const AssetTableWidget&) = delete;

    void ClearSelected();
    void Initialize(SiteConfigurationPtr configuration,
                    std::vector<IConfigurationPtr>::const_iterator &first_avail,
                    const QString &AssetKeySearch, QPushButton *pbOK);

private:
    void InitStyleSheet();

private slots:
    void slot_on_table_cell_clicked(int, int);

private:
    QPushButton * m_pbOK;
};

class AssetTableWidgetItem: public QTableWidgetItem
{
public:
    AssetTableWidgetItem(AssetConfigurationPtr configuration)
        : m_configuration(configuration)
    {
        this->setText(configuration->key());
    }

    AssetConfigurationPtr configuration() { return m_configuration; }

private:
    AssetConfigurationPtr m_configuration;
};

#endif // ASSETTABLEWIDGET_H
