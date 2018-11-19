#include <QDesktopWidget>
#include <QApplication>
#include "MineqWidgets/assettablewidget.h"
#include "mineqtabmanager.h"


#define MIN_SITE_TAB_SIZE 150

MineqTabManager::MineqTabManager(QTabWidget *tabWidget, QLineEdit* filter, QPushButton *pbOK)
    : m_tabWidget(tabWidget), m_filter(filter), m_pbOK(pbOK), m_AllSites(new SiteConfiguration())
{

}

void MineqTabManager::InitSitesConfig(TUPSites &&sitesList) {
    std::vector<AssetConfigurationPtr> lAllAssets;

    m_sitesList = std::move(sitesList);
    // all assets to show in one common list:
    lAllAssets.clear();
    m_sitesList->ForEach<SiteConfiguration>([&lAllAssets](const SiteConfigurationPtr &el) {
        lAllAssets.reserve(lAllAssets.size() + el->Assets().Size());
        el->Assets().ForEach<AssetConfiguration>(
                    [&lAllAssets](const AssetConfigurationPtr &el2) {
            lAllAssets.push_back(el2);
        });
    });
    m_AllSites->description("All sites");
    m_AllSites->Assets().CLear();
    std::sort(lAllAssets.begin(), lAllAssets.end(), [](const AssetConfigurationPtr el1, const AssetConfigurationPtr el2) {
        return QString::compare(el1->key(), el2->key(), Qt::CaseInsensitive) < 0;
    });
    for(auto &el : lAllAssets) {
        m_AllSites->Assets().Add(el);
    }
}

void MineqTabManager::AddOneCentralWidget(QWidget *widget, const QString &tabTitle)
{
    m_filter->setVisible(false);
    ClearTabs();
    SetTabStyle(1);
    m_tabWidget->addTab(widget, tabTitle);
}

void MineqTabManager::AddTableWidget()
{
    auto lambda_add = [this](const SiteConfigurationPtr &site){
        AssetTableWidget* assetTable = new AssetTableWidget();
        assetTable->Initialize(site, m_pbOK);
        m_tabWidget->addTab(assetTable, site->description());
    };
    ClearTabs();
    m_filter->setVisible(true);
    SetTabStyle(m_sitesList->Size());
    // all sites in one list:
    lambda_add(m_AllSites);
    for (int i = 0; i < m_sitesList->Size(); ++i) {
        lambda_add(m_sitesList->Item<SiteConfiguration>(i));
    }
}

void MineqTabManager::SetTabStyle(int tabCount)
{
    QString strTabBarStyle ("QTabWidget::pane"
                            "{"
                                "border:none;"
                                "margin-left: 40px;"
                                "margin-right: 40px;"
                                "margin-bottom: 40px;"
                            "}"
                            "QTabBar::tab:selected"
                            "{"
                                "color: rgb(106, 158, 236);"
                                "text-decoration: underline;"
                            "}"
                            /* just want to make tab buttons bigger...
                            "QTabBar::QToolButtons"
                            "{"
                              "width: 50px;"
                            "}"
                            "QTabBar::scroller"
                            "{"
                              "width: 100px;"
                            "}"
                            */
                            "QTabBar::tab"
                            "{"
                                "border: none;"
                                "height: 50px;"
                                "background-color: #4A5054;"
                                "color: white;");

    QRect rec = QApplication::desktop()->screenGeometry();
    int iSiteSize = (rec.width()) / tabCount;
    strTabBarStyle += "width:";
    strTabBarStyle += iSiteSize > MIN_SITE_TAB_SIZE ? QString::number(iSiteSize) : QString::number(MIN_SITE_TAB_SIZE);
    strTabBarStyle += "}";

    m_tabWidget->setStyleSheet(strTabBarStyle);
}

void MineqTabManager::ClearTabs()
{
    for (int i = 0; i < m_tabWidget->count(); ++i) {
        QWidget* tabElement = m_tabWidget->widget(i);
        tabElement->disconnect();
        delete tabElement;
    }
    m_tabWidget->clear();
}
