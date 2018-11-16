#ifndef ASSETTABLEWIDGET_H
#define ASSETTABLEWIDGET_H

#include <QTableWidget>
#include <memory>
#include <QSharedPointer>

#include "Configurations/assetconfiguration.h"
#include "Configurations/jsonconfiguration.h"

class AssetTableWidget : public QTableWidget
{
    Q_OBJECT

public:
    AssetTableWidget(QWidget *parent = nullptr);
    AssetTableWidget(const AssetTableWidget &) = delete;
    AssetTableWidget& operator = (const AssetTableWidget&) = delete;

    void Initialize(SiteConfigurationPtr configuration);

    JsonConfigurationPtr configuration() {
        return std::dynamic_pointer_cast<JsonConfiguration>(m_configuration);
    }

private:
    void InitStyleShit();

private:
    std::shared_ptr<JsonConfiguration> m_configuration;
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
