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

    SchemaWidget* createSchemaWidget(QWidget *parent);

    int hasCapability(Capability capability);

    int importDatabase();
    int exportDatabase();
    int clearDatabase();

    int createTable(QString table);
    int removeTable(QString table);
    int renameTable(QString from, QString to);

    int addColumn(QString table);
    int removeColumn(QString table, QString column);

private:
    ExtensionTab *m_toolsTab;
    QToolBar m_toolsToolBar;
    MySQLTools m_toolsWidget;

    QAction *m_backupAction;
    QAction *m_restoreAction;
    QAction *m_clearAction;
};

#endif // MYSQLEXTENSION_H
