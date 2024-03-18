#include "dbmanager.h"
#include <QDebug>
#include <QSqlError>
#include <QCoreApplication>

DbManager* DbManager::instance = nullptr;
QSqlDatabase DbManager::database;

DbManager::DbManager(QObject *parent) : QObject(parent)
{
    // Initialize the database object with a unique connection name, as to avoid conflict with overwriting the same database multiple times!
    database = QSqlDatabase::addDatabase("QSQLITE", QString::number((quintptr)this, 16));

    // Set the database name and path
    const QString dbName = "Module.db";
    const QString dbPath = QString("%1/data/%2").arg(QCoreApplication::applicationDirPath(), dbName);
    database.setDatabaseName(dbPath);

    qDebug() << "Database file path: " << database.databaseName();

    // Open the database connection
    openDatabaseConnection();

    // Initialize the database tables
    // initializeDatabase(database);
}

DbManager::~DbManager()
{
    // Close the database connection
    database.close();
}
/* // Add the SQLite driver
QSqlDatabase DbManager::database = QSqlDatabase::addDatabase("QSQLITE"); */


// Get the instance of the DbManager Object
DbManager* DbManager::getInstance()
{
    if (!instance)
        instance = new DbManager();
    return instance;
}

// Get the database object
QSqlDatabase &DbManager::getDatabase()
{
    return database;
}

/*
 void DbManager::initializeDatabase(QSqlDatabase &database)
{}
*/

void DbManager::openDatabaseConnection()
{
    // Check if the database connection is already open
    if (database.isOpen())
    {
        qDebug() << "Database is already open.";
        return;
    }

    // Add the SQLite driver if not already added
    if (!QSqlDatabase::contains())
    {
        QSqlDatabase::addDatabase("QSQLITE");
    }
    // Set the database name
    // executable will be in /build, so use a relative path from there to find find db in directory data
    database.setDatabaseName("../data/Module.db");
    // Open the database
    if (!database.open())
    {
        qDebug() << "Error: Unable to open database connection";
        qDebug() << "Database error:" << database.lastError().text(); // Log the error
        return;
    }

    qDebug() << "Database connection is open";
}

QSqlQueryModel* DbManager::displayDatabaseInTable(QTableView *tableView, QSqlDatabase &database)
{
    // Create a QSqlQueryModel to hold data
    QSqlQueryModel *model = new QSqlQueryModel();

    // Set the bullet point character
    // possible characters: ✓ ✔ ● ☑ ✗ ☒ ✖ ◇ ◆  ◉ ◈
    const QString bulletPoint = "✖";    // make this configurable

    // QString queryStr = "SELECT * FROM \"Module\""; // Make table variable!!! So function can be used for variaty of tables
    QString queryStr = QStringLiteral(R"(
    SELECT
        Module,
        Abbreviation,
        Semester,
        strftime('%d/%m/%Y', Start) AS Start,
        strftime('%d/%m/%Y', End) AS End,
        Minutes,
        CAST(Minutes / 60.0 AS REAL) AS Hours,
        Note,
        CASE WHEN SOK THEN ? ELSE '' END AS SOK,
        CASE WHEN TOK THEN ? ELSE '' END AS TOK,
        replace(hex(zeroblob(ASS)), '00', ?) AS ASS,  -- Solution from https://stackoverflow.com/questions/11568496/how-to-emulate-repeat-in-sqlite
        CASE WHEN LAB THEN ? ELSE '' END AS LAB,
        ECTS,
        (SELECT Status FROM Status WHERE StatusID = Module.StatusID) AS Status
    FROM
        Module;
)");

    // Create a QSqlQuery and set the query string
    QSqlQuery query(database);
    query.prepare(queryStr);

    // Bind the bullet point character to the parameters in the query
    query.bindValue(0, bulletPoint);
    query.bindValue(1, bulletPoint);
    query.bindValue(2, bulletPoint);
    query.bindValue(3, bulletPoint);

    // Execute the query
    query.exec();

    // Set the query to the model
    model->setQuery(query);

    // Debug Statements
    qDebug() << "Number of rows:" << model->rowCount();
    qDebug() << "Number of columns:" << model->columnCount();

    // Set the model for the table view
    tableView->setModel(model);

    // Resize columns to contents
    tableView->resizeColumnsToContents();

    return model;
}

/*
void DbManager::updateQueryModel(QSqlQueryModel *queryModel)
{
    QSqlQuery query(QString("SELECT * FROM Module"), getDatabase());
    if (!query.exec()) {
        qDebug() << "Error: Unable to execute query";
        qDebug() << "Last error: " << query.lastError();
        return;
    }

    queryModel->setQuery(query);
}
*/

void DbManager::insertModule(const Module &module, QSqlDatabase &database)
{
    QSqlQuery query(database); // Pass the database object to the QSqlQuery constructor

    if (!database.isOpen())
    {
        qDebug() << "Error: Database not open";
        return;
    }

    query.prepare("INSERT INTO Module (Module, Abbreviation, Semester, Start, End, Minutes, Note, SOK, TOK, ASS, LAB, ECTS, StatusID) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    // Bind the values to the query
    query.addBindValue(module.getLongName());
    query.addBindValue(module.getShortName());
    query.addBindValue(module.getSemester());
    query.addBindValue(module.getStartDate());
    query.addBindValue(module.getEndDate());
    query.addBindValue(module.getTimeMin());
    query.addBindValue(module.getNote());
    query.addBindValue(module.getSok());
    query.addBindValue(module.getTok());
    query.addBindValue(module.getAss());
    query.addBindValue(module.getLab());
    query.addBindValue(module.getEcts());
    query.addBindValue(module.getStatusID());
    // query.addBindValue(module.getStatusID());

    qDebug() << "Inserting module with abbreviation: " << module.getShortName();
    qDebug() << "Executing SQL: " << query.executedQuery();

    if (!query.exec())
    {
        qDebug() << "Error: Unable to insert module into database";
        qDebug() << "Last error: " << query.lastError();
    }
}

Module DbManager::selectModule(const QString &abbreviation)
{
    QSqlQuery query(QString("SELECT * FROM Module WHERE Abbreviation = '%1'").arg(abbreviation), getDatabase());

    if (!query.exec())
    {
        qDebug() << "Failed to execute query: " << query.lastError();
        return Module();
    }

    if (!query.next())
    {
        qDebug() << "Failed to select module with abbreviation" << abbreviation;
        return Module();
    }

    // Retrieve the module data from the query and create a new Module object
    QString shortName = query.value("Abbreviation").toString();
    QString longName = query.value("Module").toString();
    int semester = query.value("Semester").toInt();
    QDate startDate = query.value("Start").toDate();
    QDate endDate = query.value("End").toDate();
    int timeMin = query.value("Minutes").toInt();
    double note = query.value("Note").toDouble();
    int ects = query.value("ECTS").toInt();
    bool sok = query.value("SOK").toBool();
    bool tok = query.value("TOK").toBool();
    int ass = query.value("ASS").toInt();
    bool lab = query.value("LAB").toBool();
    // QString status = query.value("Status").toString();
    int statusID = query.value("StatusID").toInt();

    return Module(shortName, longName, semester, startDate, endDate, timeMin, note, ects, sok, tok, ass, lab, statusID);
}

bool DbManager::deleteModule(const QString &abbreviation, QSqlDatabase &database)
{
    if (!database.isOpen())
    {
        qDebug() << "Error: Database not open";
        return false;
    }

    QSqlQuery query(database);
    query.prepare("DELETE FROM Module WHERE Abbreviation = ?");
    query.addBindValue(abbreviation);

    if (!query.exec())
    {
        qDebug() << "Error: Unable to delete module from database";
        qDebug() << "Last error: " << query.lastError();
        return false;
    }

    return true;
}

bool DbManager::updateModule(const Module &module, QSqlDatabase &database)
{
    if (!database.isOpen())
    {
        qDebug() << "Error: Database not open";
        return false;
    }

    QSqlQuery query(database);
    query.prepare("UPDATE Module SET "
                  "Module = ?, "
                  "Semester = ?, "
                  "Start = ?, "
                  "End = ?, "
                  "Minutes = ?, "
                  "Note = ?, "
                  "SOK = ?, "
                  "TOK = ?, "
                  "ASS = ?, "
                  "LAB = ?, "
                  "ECTS = ?, "
                  "StatusID = ? "
                  "WHERE Abbreviation = ?");

    // Bind the values to the query
    query.addBindValue(module.getLongName());
    query.addBindValue(module.getSemester());
    query.addBindValue(module.getStartDate());
    query.addBindValue(module.getEndDate());
    query.addBindValue(module.getTimeMin());
    query.addBindValue(module.getNote());
    query.addBindValue(module.getSok());
    query.addBindValue(module.getTok());
    query.addBindValue(module.getAss());
    query.addBindValue(module.getLab());
    query.addBindValue(module.getEcts());
    query.addBindValue(module.getStatusID());
    query.addBindValue(module.getShortName());

    qDebug() << "Updating module with abbreviation: " << module.getShortName();
    qDebug() << "Executing SQL: " << query.executedQuery();

    if (!query.exec())
    {
        qDebug() << "Error: Unable to update module in database";
        qDebug() << "Last error: " << query.lastError();
        return false;
    }

    return true;
}

QSqlQueryModel* DbManager::getModuleDetails(const QString& abbreviation)
{
    QSqlQuery query(database); // Assuming database is a member variable of DbManager

    // Prepare the query with placeholder for abbreviation
    query.prepare("SELECT * FROM Module WHERE Abbreviation = :Abbreviation");
    
    // Bind the abbreviation parameter
    query.bindValue(":Abbreviation", abbreviation);
    
    // Execute the query
    if (!query.exec())
    {
        qDebug() << "Error executing query:" << query.lastError().text();
        return nullptr; // Return null if query execution fails
    }

    // Create a QSqlQueryModel to hold the query result
    QSqlQueryModel* model = new QSqlQueryModel();
    
    // Set the query to the model
    model->setQuery(query);
    
    // Check if there were any errors in setting the query
    if (model->lastError().isValid())
    {
        qDebug() << "Error setting query to model:" << model->lastError().text();
        delete model; // Clean up and delete the model
        return nullptr;
    }

    return model;
}
