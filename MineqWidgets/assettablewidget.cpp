
#include <QString>
#include <QHeaderView>
#include <QTabWidget>
#include <QPushButton>

#include "assettablewidget.h"


#define COLUMN_COUNT 3

AssetTableWidget::AssetTableWidget(QWidget *parent)
    : QTableWidget(parent)
{
    QFont fnt;
    fnt.setPointSize(16);
    fnt.setFamily("Arial");
    this->setFont(fnt);
}

AssetTableWidget::~AssetTableWidget()
{
    delete m_configuration;
}


void AssetTableWidget::Initialize(SiteConfiguration* configuration)
{
    JsonConfiguration* assetConfig = new JsonConfiguration();
    assetConfig->InsertConfiguration("", configuration);

    m_configuration = assetConfig;

    this->setColumnCount(COLUMN_COUNT);
    this->setRowCount((configuration->Assets().Size() + COLUMN_COUNT - 1) / COLUMN_COUNT);

    InitStyleShit();

    int rowToPast = -1;
    int colmToPast = 0;
    for (int i = 0; i < configuration->Assets().Size(); i++) {
        rowToPast += i % COLUMN_COUNT == 0 ? 1 : 0;
        colmToPast = i % COLUMN_COUNT == 0 ? 0 : colmToPast;

        AssetTableWidgetItem* assetCell = new AssetTableWidgetItem(configuration->Assets().Item(i));
        assetCell->setTextAlignment(Qt::AlignCenter);
        this->setItem(rowToPast, colmToPast, assetCell);

        ++colmToPast;
    }

    // need to disable empty row selection
    for (int i = colmToPast; i < COLUMN_COUNT; ++i) {
        QTableWidgetItem* assetCell = new QTableWidgetItem();
        assetCell->setFlags(Qt::NoItemFlags);
        this->setItem(rowToPast, colmToPast, assetCell);

        ++colmToPast;
    }
}

void AssetTableWidget::InitStyleShit()
{
    this->setShowGrid(false);
    QString strTableStyle ("QTableView::item"
                           "{"
                                "border: 3px solid white;"
                           "}"
                           "QTableWidget::item:selected"
                           "{"
                                "background-color: rgb(106, 158, 236);"
                           "}"
                           "QTableWidget"
                           "{"
                                "border: none;"
                                "background-color: rgb(218, 224, 229);"
                           "}");
    this->setStyleSheet(strTableStyle);
    this->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color:white; color: white; border: none;}");

    this->verticalHeader()->setVisible(false);
    this->setSelectionBehavior(QAbstractItemView::SelectItems);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);

    this->setHorizontalHeaderItem(0, new QTableWidgetItem(""));
    this->setHorizontalHeaderItem(1, new QTableWidgetItem(""));
    this->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    this->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    this->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    QHeaderView* headerView = this->verticalHeader();
    headerView->setDefaultSectionSize(50);
}

AssetTableWidgetItem::~AssetTableWidgetItem()
{
    delete m_configuration;
}
