#ifndef MYSQLEXTENSION_H
#define MYSQLEXTENSION_H

#include <QList>
#include <QSqlDatabase>
#include <QWidget>

#include "Extension.h"
#include "MySQLTools.h"

class MySQLExtension : public Extension
{
Q_OBJECT

public:
    explicit MySQLExtension(QObject *parent = 0, QSqlDatabase *database = new QSqlDatabase());
    ~MySQLExtension();

private:
    ExtensionTab *m_toolsTab;
    QToolBar m_toolsToolBar;
    MySQLTools m_toolsWidget;

    QAction *m_backupAction;
    QAction *m_restoreAction;
    QAction *m_clearAction;
};

#endif // MYSQLEXTENSION_H
