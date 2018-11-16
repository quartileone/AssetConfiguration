#ifndef MINEQTABMANAGER_H
#define MINEQTABMANAGER_H

#include <QTabWidget>
#include <QLineEdit>

class MineqTabManager
{
public:
    MineqTabManager(QTabWidget* tabWidget, QLineEdit *filter);

    void AddMineqWidget(QWidget* widget, QString text);

    void SetTabCount(int tabCount);

    void ClearTabs();

    void OnSearchTextChanged(const QString &strNewText);

private:
    QTabWidget* m_tabWidget;
    QLineEdit * m_filter;
};

#endif // MINEQTABMANAGER_H
