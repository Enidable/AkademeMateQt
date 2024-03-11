#include "dbmanager.h"
#include <QDebug>
#include <QSqlError>

DbManager::DbManager(QObject *parent) : QObject(parent)
{
    // Initialize the database object
    database = QSqlDatabase::addDatabase("QSQLITE");
}

DbManager::~DbManager()
{
// Close the database connection
    database.close();
}
// Add the SQLite driver
QSqlDatabase DbManager::database = QSqlDatabase::addDatabase("QSQLITE");

// Get the database object
QSqlDatabase& DbManager::getDatabase()
{
    return database;
}

void DbManager::openDatabaseConnection()
{
    // Check if the database connection is already open
    if (database.isOpen()) {
        return;
    }
   
    //database = QSqlDatabase::addDatabase("QSQLITE");
    // Set the database name
    // executable will be in /build, so use a relative path from there to find find db in directory data
    database.setDatabaseName("../data/Module.db");
    // Open the database
    if (!database.open()) {
        qDebug() << "Error: Unable to open database connection";
        return;
    }

    qDebug() << "Database connection is open";
}

void DbManager::displayDatabaseInTable(QTableView* tableView)
{
    // Create a QSqlQueryModel to hold your data
    QSqlQueryModel *model = new QSqlQueryModel();

    QString queryStr = "SELECT * FROM \"Module\"";

    qDebug() << "Is the database open?" << database.isOpen();  // Debug Statement 1

    // Set the query to the model
    model->setQuery(queryStr);

    // Debug Statement 2
    if (!model->query().executedQuery().isEmpty()) {
        qDebug() << "Query executed successfully:" << model->query().executedQuery();
    } else {
        qDebug() << "Error executing query:" << model->query().lastError().text();
        return;
    }

    // Debug Statements 3 and 4
    qDebug() << "Number of rows:" << model->rowCount();
    qDebug() << "Number of columns:" << model->columnCount();

    // Set the model for the table view
    tableView->setModel(model);
    qDebug() << "Model set";

    // Resize columns to contents
    tableView->resizeColumnsToContents();
}
