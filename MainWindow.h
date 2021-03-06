#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "DatabaseConnectionWidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionNew_Connection_triggered();

    void on_actionQuit_triggered();

    void on_actionAbout_Qt_triggered();

    void on_actionAbout_QuteSql_triggered();

    void on_databaseConnectionsTabWidget_tabCloseRequested(int index);

    void on_databaseConnectionsTabWidget_currentChanged(int index);

    void on_actionClose_Connection_triggered();

    void on_actionRefresh_triggered();

    void on_actionSplit_SQL_File_triggered();

    void on_actionImport_Database_triggered();

    void on_actionExport_Database_triggered();

    void on_actionClear_Database_triggered();

signals:
    void statusEvent(const QString&);

private:
    Ui::MainWindow *ui;

    QList<DatabaseConnectionWidget> m_databaseConnections;

    DatabaseConnectionWidget *m_currentDatabase;
};

#endif // MAINWINDOW_H
