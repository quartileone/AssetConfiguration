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
    auto & lAllAssets = m_AllSites->Assets();
    m_sitesList = std::move(sitesList);
    // all assets to show in one common list:
    lAllAssets.clear();
    m_sitesList->ForEach<SiteConfiguration>([&lAllAssets](const SiteConfigurationPtr &el) {
        auto old_size = lAllAssets.size();
        lAllAssets.reserve(old_size + el->Assets().size());
        std::transform(el->Assets().cbegin(), el->Assets().cend(), std::back_inserter(lAllAssets),
                       [](const IConfigurationPtr &el) {
            return std::dynamic_pointer_cast<AssetConfiguration>(el);
        });

        if (old_size > 0) {
            std::inplace_merge(lAllAssets.begin(), lAllAssets.begin() + old_size, lAllAssets.end(),
                               lAllAssets.GetComparator());
        }
    });
    m_AllSites->description("All sites");
}

void MineqTabManager::AddOneCentralWidget(QWidget *widget, const QString &tabTitle, bool hideFilter)
{
    if (hideFilter)
        m_filter->setVisible(false);
    ClearTabs();
    SetTabStyle(1);
    m_tabWidget->addTab(widget, tabTitle);
}

void MineqTabManager::ChangeTabIndex(int index) {
    if (m_prevTabIndex != index) {
        QTableWidget* tableWidget = (QTableWidget*)m_tabWidget->currentWidget();
        tableWidget->clearSelection();
        tableWidget->setCurrentCell(-1, -1);
        m_pbOK->setStyleSheet("border: none; color: #FFFFFF; background-color: rgba(106, 158, 236, 50); margin-right: 40px;");
        m_pbOK->setEnabled(false);
    }
    m_prevTabIndex = index;
}

bool MineqTabManager::AddTableWidget()
{
    QString strLowerCase = m_filter->text().toLower();
    auto lambda_add = [this, &strLowerCase](const SiteConfigurationPtr &site) {
        auto &AssetVec = site->Assets();
        std::vector<IConfigurationPtr>::const_iterator it;
        if (AssetVec.size() == 0)
            return false;
        if (strLowerCase == "")
            it = AssetVec.cbegin();
        else
            it = std::lower_bound(AssetVec.cbegin(), AssetVec.cend(), strLowerCase,
                                  [](const IConfigurationPtr &el, const QString & val)  -> bool {
                return QString::compare(std::dynamic_pointer_cast<AssetConfiguration>(el)->key().toLower(),
                                        val, Qt::CaseSensitive) < 0;
            });
        if (it != AssetVec.cend()) {
          const auto &AssetKey = std::dynamic_pointer_cast<AssetConfiguration>(*it)->key().toLower();
          if (!AssetKey.startsWith(strLowerCase, Qt::CaseSensitive))
              return false;
          AssetTableWidget* assetTable = new AssetTableWidget();
          assetTable->Initialize(site, it, strLowerCase, m_pbOK);
          m_tabWidget->addTab(assetTable, site->description());
          assetTable->clearSelection();
          assetTable->setCurrentCell(-1, -1);
          return true;
        }
        return false;
    };
    ClearTabs();
    m_pbOK->setStyleSheet("border: none; color: #FFFFFF; background-color: rgba(106, 158, 236, 50); margin-right: 40px;");
    m_pbOK->setEnabled(false);

    m_filter->setVisible(true);
    SetTabStyle(m_sitesList->size());
    // all sites in one list:
    if (!lambda_add(m_AllSites))
        return false;
    for (size_t i = 0; i < m_sitesList->size(); ++i) {
        lambda_add(m_sitesList->Item<SiteConfiguration>(i));
    }
    m_prevTabIndex = 0;
    m_tabWidget->setCurrentIndex(0);
    return true;
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
