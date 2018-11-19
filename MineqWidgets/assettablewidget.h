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

    void Initialize(SiteConfigurationPtr configuration, QPushButton *pbOK);

    JsonConfigurationPtr configuration() {
        return std::dynamic_pointer_cast<JsonConfiguration>(m_configuration);
    }

private:
    void InitStyleSheet();

private slots:
    void slot_on_table_cell_clicked(int, int);

private:
    std::shared_ptr<JsonConfiguration> m_configuration;
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

    IConfigurationPtr configuration() { return m_configuration; }

private:
    AssetConfigurationPtr m_configuration;
};

#endif // ASSETTABLEWIDGET_H
