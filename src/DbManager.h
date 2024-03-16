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
    // Getter method for the database object
    static QSqlDatabase& getDatabase();

    // Getter method for the DbManager instance
    static DbManager* getInstance();

public slots:
    void openDatabaseConnection();
    QSqlQueryModel* displayDatabaseInTable(QTableView* tableView, QSqlDatabase &database);         // Which table??? 
    void insertModule(const Module& module,QSqlDatabase &database);
    bool deleteModule(const QString& abbreviation, QSqlDatabase& database);
    bool updateModule(const Module& module, QSqlDatabase& database);
    void initializeDatabase(QSqlDatabase &database);
    Module selectModule(const QString &abbreviation);

private:
    // Private constructor to prevent instantiation from outside the class
    explicit DbManager(QObject *parent = nullptr);

    // Private destructor to prevent destruction from outside the class
    ~DbManager();

    // Private copy constructor and copy assignment operator to prevent copying
    DbManager(const DbManager&) = delete;
    DbManager& operator=(const DbManager&) = delete;

    // Private static variable to hold the instance of the class
    static DbManager* instance;

    // Private static variable to hold the database object
    static QSqlDatabase database;

    QTableView *MainTable;
};

#endif