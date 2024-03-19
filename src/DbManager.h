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

/**
 * @brief The DbManager class handles interactions with the database.
 * 
 * The DbManager class implements the Singleton Pattern for managing the database.
 * This class provides methods for managing the database, including opening connections,
 * displaying data in tables, inserting, updating, and deleting modules, as well as retrieving
 * module details and initializing the database.
 */

class DbManager : public QObject
{
    Q_OBJECT

public:
    // Getter method for the database object
    static QSqlDatabase& getDatabase();

/**
 * @brief Implements the Singleton Pattern for managing the database.
 * 
 * Ensures only one instance of DbManager exists throughout the application's lifecycle.
 * Provides a static method, getInstance(), for accessing the single instance.
 */
    // Getter method for the DbManager instance
    static DbManager* getInstance();

public slots:

    /**
     * @brief Opens a connection to the database.
     * 
     * This slot opens a connection to the database if one is not already open.
     */
    void openDatabaseConnection();

    /**
     * @brief Displays data from the database in the MainTable TableView.
     * 
     * This slot populates the provided table view with data retrieved from the database.
     * 
     * @param tableView Pointer to the QTableView where the data will be displayed.
     * @param database Reference to the QSqlDatabase object representing the database.
     * @return QSqlQueryModel* A pointer to the QSqlQueryModel containing the query result.
     */
    QSqlQueryModel* displayDatabaseInTable(QTableView* tableView, QSqlDatabase &database);         // MainTable 

    /**
     * @brief Retrieves detailed information about a module from the database.
     * 
     * This slot retrieves detailed information about a module, such as its name, time categories of the corresponding performance assessments,
     * performance assessment, time investment and time estimation.
     * 
     * @param abbreviation The abbreviation of the module for which details are to be retrieved.
     * @return QSqlQueryModel* A pointer to the QSqlQueryModel containing the module details.
     */
    QSqlQueryModel* getModuleDetails(const QString& abbreviation);

    /**
     * @brief Inserts a new module into the database.
     * 
     * This slot inserts a new module into the database.
     * 
     * @param module The Module object representing the module to be inserted.
     * @param database Reference to the QSqlDatabase object representing the database.
     */
    void insertModule(const Module& module,QSqlDatabase &database);

    /**
     * @brief Deletes a module from the database.
     * 
     * This slot deletes a module from the database based on its abbreviation.
     * 
     * @param abbreviation The abbreviation of the module to be deleted.
     * @param database Reference to the QSqlDatabase object representing the database.
     * @return bool True if the module was successfully deleted, otherwise false.
     */
    bool deleteModule(const QString& abbreviation, QSqlDatabase& database);

    /**
     * @brief Updates an existing module in the database.
     * 
     * This slot updates an existing module in the database.
     * 
     * @param module The Module object representing the updated module information.
     * @param database Reference to the QSqlDatabase object representing the database.
     * @return bool True if the module was successfully updated, otherwise false.
     */
    bool updateModule(const Module& module, QSqlDatabase& database);
    
    /**
     * @brief Retrieves a module from the database based on its abbreviation.
     * 
     * This slot retrieves a module record from the database based on its abbreviation.
     * 
     * @param abbreviation The abbreviation of the module to be retrieved.
     * @return Module The Module object representing the retrieved module.
     */
    Module selectModule(const QString &abbreviation);
    public:

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
    QTableView *DetailTable;
};

#endif