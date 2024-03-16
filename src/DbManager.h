#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QTableView>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QDate>
#include "Module.h"


class DbManager : public QObject
{
    Q_OBJECT

public:
    explicit DbManager(QObject *parent = nullptr);
    ~DbManager();

    // Getter method for the database object
    static QSqlDatabase& getDatabase();

public slots:
    void openDatabaseConnection();
    QSqlQueryModel* displayDatabaseInTable(QTableView* tableView, QSqlDatabase &database);         // Which table??? 
    void insertModule(const Module& module,QSqlDatabase &database);
    bool deleteModule(const QString& abbreviation, QSqlDatabase& database);
    bool updateModule(const Module& module, QSqlDatabase& database);
    void initializeDatabase(QSqlDatabase &database);
    Module selectModule(const QString &abbreviation);
private:
    static QSqlDatabase database;
    QTableView *MainTable;
};

#endif