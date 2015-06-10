#include "MySQLExtension.h"
#include "MySQLTools.h"
#include "MySQLSchemaWidget.h"

#include <QApplication>
#include <QAction>
#include <QFileDialog>
#include <QDir>
#include <QProcess>
#include <QMessageBox>
#include <QInputDialog>
#include <QSqlQuery>
#include <QSqlError>

MySQLExtension::MySQLExtension(QObject *parent, QSqlDatabase *database) :
    Extension(parent, database)
{
    // Backup actions
    m_backupAction = new QAction(QIcon::fromTheme("document-export"), QT_TR_NOOP("&Export..."), &m_toolsToolBar);
    m_backupAction->setStatusTip(QT_TR_NOOP("Export database"));
    connect(m_backupAction, SIGNAL(triggered()), &m_toolsWidget, SLOT(on_backupDatabaseButton_clicked()));

    // Restore actions
    m_restoreAction = new QAction(QIcon::fromTheme("document-import"), QT_TR_NOOP("&Import..."), &m_toolsToolBar);
    m_restoreAction->setStatusTip(QT_TR_NOOP("Import Database"));
    connect(m_restoreAction, SIGNAL(triggered()), &m_toolsWidget, SLOT(on_restoreDatabaseButton_clicked()));

    // Clear actions
    m_clearAction = new QAction(QIcon::fromTheme("edit-clear"), QT_TR_NOOP("&Clear..."), &m_toolsToolBar);
    m_clearAction->setStatusTip(QT_TR_NOOP("Clear Database"));
    connect(m_clearAction, SIGNAL(triggered()), &m_toolsWidget, SLOT(on_clearDatabaseButton_clicked()));

    // Add actions to "Tools" toolbar
    m_toolsToolBar.addAction(m_backupAction);
    m_toolsToolBar.addAction(m_restoreAction);
    m_toolsToolBar.addAction(m_clearAction);

    // Append "Tools" toolbar
    m_toolBars.append(&m_toolsToolBar);

    // Create Tools Tab
    m_toolsWidget.setDatabase(m_database);
    m_toolsTab = new ExtensionTab(&m_toolsWidget, QIcon::fromTheme("applications-utilities"), "Tools");
    m_tabs.append(m_toolsTab);

    // Connect our signals
    connect(&m_toolsWidget, SIGNAL(refreshNeeded()), this, SIGNAL(refreshNeeded()));
}

MySQLExtension::~MySQLExtension()
{
    delete m_toolsTab;
    delete m_clearAction;
    delete m_backupAction;
    delete m_restoreAction;
}

int MySQLExtension::createTable(QString table)
{
    // Run the drop query
    QSqlQuery query = m_database->exec("CREATE TABLE " + table + "(id INT NOT NULL AUTO_INCREMENT, PRIMARY KEY(id))");

    return true;
}

int MySQLExtension::removeTable(QString table)
{
    // Run the drop query
    QSqlQuery query = m_database->exec("DROP TABLE " + table);

    return true;
}

SchemaWidget* MySQLExtension::createSchemaWidget(QWidget *parent)
{
    return new MySQLSchemaWidget(parent, m_database);
}
