#ifndef ASSETTABLEWIDGET_H
#define ASSETTABLEWIDGET_H

#include <QTableWidget>
#include <QSharedPointer>

#include "Configurations/assetconfiguration.h"
#include "Configurations/jsonconfiguration.h"

class AssetTableWidget
        : public QTableWidget
{
    Q_OBJECT

public:
    AssetTableWidget(QWidget *parent = 0);
    ~AssetTableWidget();

    void Initialize(SiteConfiguration* configuration);

    JsonConfiguration* configuration() { return static_cast<JsonConfiguration *>(m_configuration); }

private:
    void InitStyleShit();

private:
    IConfiguration* m_configuration;
};


class AssetTableWidgetItem
        : public QTableWidgetItem
{
public:
    AssetTableWidgetItem(AssetConfiguration* configuration)
        : m_configuration(configuration)
    {
        this->setText(configuration->description());
    }

    ~AssetTableWidgetItem()
    {
        delete m_configuration;
    }

    AssetConfiguration* configuration() { return static_cast<AssetConfiguration *>(m_configuration); }

private:
    IConfiguration* m_configuration;
};

#endif // ASSETTABLEWIDGET_H
