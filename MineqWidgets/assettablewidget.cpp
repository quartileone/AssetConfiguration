
#include <QString>
#include <QHeaderView>
#include <QTabWidget>

#include "assettablewidget.h"


#define COLUMN_COUNT 3

AssetTableWidget::AssetTableWidget(QWidget *parent)
    : QTableWidget(parent), m_configuration(new JsonConfiguration())
{
    QFont fnt;
    fnt.setPointSize(16);
    fnt.setFamily("Arial");
    this->setFont(fnt);
}

void AssetTableWidget::slot_on_table_cell_clicked(int, int)
{
    QTableWidgetItem* tableItem = item(currentRow(), currentColumn());

    if (tableItem) {
        m_pbOK->setStyleSheet("border: none; color: #FFFFFF; background-color: rgb(106, 158, 236); margin-right: 40px");
        m_pbOK->setEnabled(true);
    }
}

void AssetTableWidget::Initialize(SiteConfigurationPtr configuration,
                                  std::vector<IConfigurationPtr>::const_iterator &first_avail,
                                  const QString &AssetKeySearch, QPushButton *pbOK)
{
    m_configuration->InsertConfiguration("", configuration);
    auto &AssetVec = configuration->Assets();
    auto it = first_avail;
    size_t assetCounter;
    m_pbOK = pbOK;
    connect(this
            , SIGNAL(cellClicked(int, int))
            , this
            , SLOT(slot_on_table_cell_clicked(int,int)));

    this->setColumnCount(COLUMN_COUNT);
    for (assetCounter = 0; it < AssetVec.cend(); it++, assetCounter++) {
        auto Asset = std::dynamic_pointer_cast<AssetConfiguration>(*it);
        if (AssetKeySearch != "") {
            if (!Asset->key().toLower().startsWith(AssetKeySearch, Qt::CaseSensitive))
                break;
        }
    }
    this->setRowCount((assetCounter + COLUMN_COUNT - 1) / COLUMN_COUNT);

    InitStyleSheet();

    int rowToPaste = -1;
    int colToPaste = 0;
    it = first_avail;
    for (size_t i = 0; i < assetCounter; it++, i++) {
        auto Asset = std::dynamic_pointer_cast<AssetConfiguration>(*it);
        rowToPaste += i % COLUMN_COUNT == 0 ? 1 : 0;
        colToPaste = i % COLUMN_COUNT == 0 ? 0 : colToPaste;
        AssetTableWidgetItem* assetCell = new AssetTableWidgetItem(Asset);
        assetCell->setTextAlignment(Qt::AlignCenter);
        this->setItem(rowToPaste, colToPaste, assetCell);

        ++colToPaste;
    }
    // need to disable empty row selection
    for (int i = colToPaste; i < COLUMN_COUNT; ++i) {
        QTableWidgetItem* assetCell = new QTableWidgetItem();
        assetCell->setFlags(Qt::NoItemFlags);
        this->setItem(rowToPaste, colToPaste, assetCell);

        ++colToPaste;
    }
}

void AssetTableWidget::InitStyleSheet()
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

