#include "OpenConnectionDialog.h"
#include "ui_OpenConnectionDialog.h"

#include <QSqlDatabase>
#include <QFileDialog>
#include <QSettings>
#include <QDebug>

OpenConnectionDialog::OpenConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenConnectionDialog)
{
    ui->setupUi(this);

    // Load saved connections
    QSettings settings;
    int size = settings.beginReadArray("connections");
    for (int i = 0; i < size; i++) {
        settings.setArrayIndex(i);

        // Add connection from settings to list
        SavedConnection *connection = new SavedConnection;
        connection->name = settings.value("name").toString();
        connection->database = settings.value("database").toString();
        connection->driver = settings.value("driver").toString();
        connection->hostname = settings.value("hostname").toString();
        connection->username = settings.value("username").toString();
        connection->password = settings.value("password").toString();
        connection->port = settings.value("port").toInt();

        // Append to saved connections
        savedConnections.append(connection);

        // Add item to combo box
        ui->connectionsComboBox->addItem(connection->name, savedConnections.length());
    }
    settings.endArray();

    // Setup list of drivers for selection
    foreach (QString driver, QSqlDatabase::drivers()) {
        ui->driverCombo->addItem(driver);
    }
}

OpenConnectionDialog::~OpenConnectionDialog()
{
    delete ui;
}

QString OpenConnectionDialog::getName()
{
    if (getDriver() == "QMYSQL" || getDriver() == "QMYSQL3") {
        return QString(getDatabase() + " - " + getUsername() + "@" + getHostname() + ":" + QString::number(getPort()));
    }

    QUrl url(ui->databaseEdit->text());

    return url.fileName();
}

QString OpenConnectionDialog::getDriver()
{
    return ui->driverCombo->currentText();
}

QString OpenConnectionDialog::getHostname()
{
    return ui->hostnameEdit->text();
}

QString OpenConnectionDialog::getDatabase()
{
    return ui->databaseEdit->text();
}

QString OpenConnectionDialog::getUsername()
{
    return ui->usernameEdit->text();
}

QString OpenConnectionDialog::getPassword()
{
    return ui->passwordEdit->text();
}

int OpenConnectionDialog::getPort()
{
    return ui->portSpinBox->text().toInt();
}

bool OpenConnectionDialog::getSaveConnection()
{
    return ui->saveCheckBox->isChecked();
}

void OpenConnectionDialog::on_driverCombo_currentIndexChanged(const QString &arg1)
{
    // Disable fields if using SQLite
    if (arg1 == "QSQLITE") {
        ui->fileButton->setEnabled(true);
        ui->usernameEdit->setEnabled(false);
        ui->passwordEdit->setEnabled(false);
        ui->hostnameEdit->setEnabled(false);
        ui->portSpinBox->setEnabled(false);
    }

    // Enable all fields if using MySQL
    else if (arg1 == "QMYSQL" || arg1 == "QMYSQL3") {
        ui->fileButton->setEnabled(false);
        ui->usernameEdit->setEnabled(true);
        ui->passwordEdit->setEnabled(true);
        ui->hostnameEdit->setEnabled(true);
        ui->portSpinBox->setEnabled(true);
        ui->portSpinBox->setValue(3306);
    }
}

void OpenConnectionDialog::on_fileButton_clicked()
{
    // Show dialog and get filename
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/jana", tr("SQL Files (*.sqlite)"));

    ui->databaseEdit->setText(fileName);
}

void OpenConnectionDialog::on_okButton_clicked()
{
    if (ui->saveCheckBox->isChecked()) {

        // Add connection from settings to list
        SavedConnection *newConnection = new SavedConnection;
        newConnection->name = getName();
        newConnection->database = getDatabase();
        newConnection->driver = getDriver();
        newConnection->hostname = getHostname();
        newConnection->username = getUsername();
        newConnection->password = getPassword();
        newConnection->port = getPort();

        // Append to saved connections
        savedConnections.append(newConnection);

        // Save connections
        QSettings settings;
        settings.beginWriteArray("connections");

        int i = 0;
        foreach(SavedConnection* connection, savedConnections)
        {
            settings.setArrayIndex(i);

            // Add connection from settings to list
            settings.setValue("name", connection->name);
            settings.setValue("database", connection->database);
            settings.setValue("driver", connection->driver);
            settings.setValue("hostname", connection->hostname);
            settings.setValue("username", connection->username);
            settings.setValue("password", connection->password);
            settings.setValue("port", connection->port);

            i++;
        }
        settings.endArray();
    }
}

void OpenConnectionDialog::on_connectionsComboBox_currentIndexChanged(int index)
{
    SavedConnection *connection = savedConnections.at(index);

    int driver = ui->driverCombo->findText(connection->driver);
    ui->driverCombo->setCurrentIndex(driver);
    ui->databaseEdit->setText(connection->database);
    ui->usernameEdit->setText(connection->username);
    ui->passwordEdit->setText(connection->password);
    ui->hostnameEdit->setText(connection->hostname);
    ui->portSpinBox->setValue(connection->port);
}
