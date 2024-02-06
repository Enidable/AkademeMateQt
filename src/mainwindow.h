#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "StudyTimer.h"
#include <QTableWidget>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QStringList>
#include <QtSql/QSqlError>
#include <QDebug>

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
    void resetTimer();  // Declaration of the resetTimer function
    // Tables
    void displayDatabaseInTable();
private:
    Ui::MainWindow *ui;
    StudyTimer *studyTimer;
    QTableWidget *lapsTable;
    QTableWidget *MainTable;
};
#endif // MAINWINDOW_H
