#ifndef MINEQTABMANAGER_H
#define MINEQTABMANAGER_H

#include <map>
#include <QTabWidget>
#include <QLineEdit>
#include <QPushButton>
#include "Configurations/assetconfiguration.h"

class MineqTabManager
{
public:
    MineqTabManager(QTabWidget* tabWidget, QLineEdit *filter, QPushButton *pbOK);

    void InitSitesConfig(TUPSites &&sitesList);
    void AddOneCentralWidget(QWidget* widget, const QString &tabTitle, bool hideFilter = true);
    bool AddTableWidget();
    void SetTabStyle(int tabCount);
    void ClearTabs();
    void OnSearchTextChanged(const QString &strNewText);
    void ChangeTabIndex(int index);
    void DisableOKButton();
private:
    QTabWidget           * m_tabWidget;
    QLineEdit            * m_filter;
    QPushButton          * m_pbOK;
    TUPSites             m_sitesList;
    SiteConfigurationPtr m_AllSites;
    int                  m_prevTabIndex;
};

#endif // MINEQTABMANAGER_H
