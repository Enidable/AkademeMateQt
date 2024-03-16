#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "StudyTimer.h"
#include <QTableWidget>
#include <QTableView>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QStringList>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQueryModel>
#include <QDebug>
#include "ColorDelegate.h"
#include "DbManager.h"
#include "DbInputWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Timer
    void updateTimerLabel(int seconds);
    void updateLapsTable(const QStringList &laps);

    // Tables
    void addModuleclicked();
    void editModuleclicked();
    void deleteModuleclicked();
    void onRowClicked(const QModelIndex &current, const QModelIndex &previous);
private:
    Ui::MainWindow *ui;
    StudyTimer *studyTimer;
    QTableWidget *lapsTable;
    QTableView *MainTable;
    DbManager *dbManager; // Add a pointer to a DbManager instance
    DbInputWindow *dbInputwindow;
    QSqlQueryModel *queryModel;
};
#endif // MAINWINDOW_H
