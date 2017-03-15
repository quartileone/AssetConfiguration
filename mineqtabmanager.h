#ifndef MINEQTABMANAGER_H
#define MINEQTABMANAGER_H

#include <QTabWidget>

class MineqTabManager
{
public:
    MineqTabManager(QTabWidget* tabWidget);

    void AddMineqWidget(QWidget* widget, QString text);

    void SetTabCount(int tabCount);

    void ClearTabs();

private:
    QTabWidget* m_tabWidget;
};

#endif // MINEQTABMANAGER_H
