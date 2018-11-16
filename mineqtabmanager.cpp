#include <QDesktopWidget>
#include <QApplication>

#include "mineqtabmanager.h"


#define MIN_SITE_TAB_SIZE 150

MineqTabManager::MineqTabManager(QTabWidget *tabWidget)
        : m_tabWidget(tabWidget)
{

}

void MineqTabManager::AddMineqWidget(QWidget *widget, QString text)
{
    m_tabWidget->addTab(widget, text);
}

void MineqTabManager::SetTabCount(int tabCount)
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
