#include "DatabaseConnectionWidget.h"
#include "ui_DatabaseConnectionWidget.h"

#include <QUrl>
#include <QDebug>

#include "Extension/MySQLExtension.h"

DatabaseConnectionWidget::DatabaseConnectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DatabaseConnectionWidget)
{
    m_extension = NULL;

    ui->setupUi(this);
}

DatabaseConnectionWidget::~DatabaseConnectionWidget()
{
    // Close this connection
    m_database.close();

    // Remove the actual database (is this necessary if I close it?)
    QSqlDatabase::removeDatabase(m_database.connectionName());

    delete m_extension;

    delete ui;
}

QSqlDatabase DatabaseConnectionWidget::database()
{
    return m_database;
}

QString DatabaseConnectionWidget::name()
{
    return m_database.connectionName();
}

QSqlError DatabaseConnectionWidget::lastError()
{
    return m_database.lastError();
}

bool DatabaseConnectionWidget::createSshTunnel(QString username, QString password, QString host, int remotePort, int localPort)
{
    //-fNg -L 16111:127.0.0.1:3306 forge@www.aussieindoorsports.com.au

    // Compile Arguments
    QStringList arguments;
    arguments << "-fNg" << QString(QString::number(localPort) + ":" + "127.0.0.1" + ":" + QString::number(remotePort)) << QString(username + "@" + host);

    // Execute SSH command
    m_sshTunnel.start("ssh", arguments);

    // Wait until finished
    if (!m_sshTunnel.waitForFinished()) {
        return false;
    }

    //  Check for exit code
    qDebug() << "SSH tunnel binded to 127.0.0.1:" << localPort << m_sshTunnel.exitCode();

    return true;
}

bool DatabaseConnectionWidget::connectToDatabase(QString name, QString driver, QString host, QString database, QString username, QString password, int port)
{
    m_database = QSqlDatabase::addDatabase(driver, name);
    m_database.setHostName(host);
    m_database.setDatabaseName(database);
    m_database.setUserName(username);
    m_database.setPassword(password);
    m_database.setPort(port);

    // Attempt to connect
    if (!m_database.open()) {
        return false;
    }

    // Setup Explorer Widget
    ui->explorerTab->setDatabase(&m_database);

    // Setup Query Widget
    ui->queryTab->setDatabase(&m_database);
    connect(ui->queryTab, SIGNAL(refreshNeeded()), this, SLOT(refresh()));

    // Setup SQL Widget
    ui->sqlTab->setDatabase(m_database);
    connect(ui->sqlTab, SIGNAL(refreshNeeded()), this, SLOT(refresh()));

    // If there's extra support for this driver, add it
    if (driver == "QMYSQL" || driver == "QMYSQL3") {

        // Create the extension (adds Tools widget, etc)
        m_extension = new MySQLExtension(this, &m_database);
        connect(m_extension, SIGNAL(refreshNeeded()), this, SLOT(refresh()));
    }

    // Setup the extensions if they exist
    if (m_extension) {

        // Add each tab provided by this extension
        QList<ExtensionTab*> tabs = m_extension->getTabs();
        QList<ExtensionTab*>::iterator i;
        for (i = tabs.begin(); i != tabs.end(); ++i) {
            ui->tabWidget->addTab((*i)->getWidget(), (*i)->getIcon(), (*i)->getLabel());
        }
    }

    return true;
}

Extension *DatabaseConnectionWidget::getExtension()
{
    return m_extension;
}

void DatabaseConnectionWidget::refresh()
{
    ui->explorerTab->refresh();
}
