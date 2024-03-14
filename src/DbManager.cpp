#include "dbmanager.h"
#include <QDebug>
#include <QSqlError>
#include <QCoreApplication>

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
    initializeDatabase(database);
}

DbManager::~DbManager()
{
// Close the database connection
//    database.close();
}
/* // Add the SQLite driver
QSqlDatabase DbManager::database = QSqlDatabase::addDatabase("QSQLITE"); */

// Get the database object
QSqlDatabase& DbManager::getDatabase()
{
    return database;
}

void DbManager::initializeDatabase(QSqlDatabase &database)
{
    // Create tables / data structure
    QSqlQuery query(database);
     if (!query.exec("CREATE TABLE IF NOT EXISTS Module ("
                   "ModuleID INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "abbreviation TEXT KEY,"
                   "fullName TEXT,"
                   "semester INTEGER,"
                   "startDate DATE,"
                   "endDate DATE,"
                   "minutes INTEGER,"
                   "note TEXT,"
                   "ECTS INTEGER,"
                   "SOK BOOLEAN,"
                   "TOK BOOLEAN,"
                   "ASS INTEGER,"
                   "LAB BOOLEAN,"
                   "Status TEXT,"
                   "UNIQUE (abbreviation))")) {
        qDebug() << "Failed to create Module table: " << query.lastError();
    } else {
        qDebug() << "Module table created or already exists.";
    }

    if (!query.exec("CREATE TABLE IF NOT EXISTS Assignment_Laboratory_report ("
                   "ModuleID INTEGER,"
                   "AssID INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "AssName TEXT,"
                   "Type TEXT,"
                   "Status TEXT,"
                   "FOREIGN KEY (ModuleID) REFERENCES Module (ModuleID))")) {
        qDebug() << "Failed to create Assignment_Laboratory_report table: " << query.lastError();
    } else {
        qDebug() << "Assignment_Laboratory_report table created or already exists.";
    }

    if (!query.exec("CREATE TABLE IF NOT EXISTS Exam ("
                   "ModuleID INTEGER,"
                   "ExamID INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "ExamName TEXT,"
                   "Status TEXT,"
                   "FOREIGN KEY (ModuleID) REFERENCES Module (ModuleID))")) {
        qDebug() << "Failed to create Exam table: " << query.lastError();
    } else {
        qDebug() << "Exam table created or already exists.";
    }

    if (!query.exec("CREATE TABLE IF NOT EXISTS Time_ASS ("
                   "AssID INTEGER,"
                   "timeType TEXT,"
                   "research INTEGER,"
                   "application INTEGER,"
                   "writeAndCreate INTEGER,"
                   "proofreading INTEGER,"
                   "timeAss INTEGER,"
                   "FOREIGN KEY (AssID) REFERENCES Assignment_Laboratory_report (AssID))")) {
        qDebug() << "Failed to create Time_ASS table: " << query.lastError();
    } else {
        qDebug() << "Time_ASS table created or already exists.";
    }

    if (!query.exec("CREATE TABLE IF NOT EXISTS Time_Exam ("
                   "ExamID INTEGER,"
                   "timeType TEXT,"
                   "studying INTEGER,"
                   "studyLetters INTEGER,"
                   "practice INTEGER,"
                   "StudyingForExam INTEGER,"
                   "exam INTEGER,"
                   "timeExam INTEGER,"
                   "FOREIGN KEY (ExamID) REFERENCES Exam (ExamID))")) {
        qDebug() << "Failed to create Time_Exam table: " << query.lastError();
    } else {
        qDebug() << "Time_Exam table created or already exists.";
    }
}

void DbManager::openDatabaseConnection()
{
    // Check if the database connection is already open
    if (database.isOpen()) {
        qDebug()<< "Database is already open.";
        return;
    }
    
    // Add the SQLite driver if not already added
    if (!QSqlDatabase::contains()) {
        QSqlDatabase::addDatabase("QSQLITE");
    }
    // Set the database name
    // executable will be in /build, so use a relative path from there to find find db in directory data
    database.setDatabaseName("../data/Module.db");
    // Open the database
    if (!database.open()) {
        qDebug() << "Error: Unable to open database connection";
        qDebug() << "Database error:" << database.lastError().text(); // Log the error
        return;
    }

    qDebug() << "Database connection is open";
}

void DbManager::displayDatabaseInTable(QTableView* tableView, QSqlDatabase &database)
{
    // Create a QSqlQueryModel to hold data
    QSqlQueryModel *model = new QSqlQueryModel();

    QString queryStr = "SELECT * FROM \"Module\"";

    // Set the query to the model using the provided database connection
    model->setQuery(queryStr, database);

    // Debug Statements
    qDebug() << "Number of rows:" << model->rowCount();
    qDebug() << "Number of columns:" << model->columnCount();

    // Set the model for the table view
    tableView->setModel(model);

    // Resize columns to contents
    tableView->resizeColumnsToContents();
}

void DbManager::insertModule(const Module& module, QSqlDatabase &database){
    QSqlQuery query(database); // Pass the database object to the QSqlQuery constructor

    if (!database.isOpen()) {
        qDebug() << "Error: Database not open";
        return;
    }

    query.prepare("INSERT INTO Module (fullName, abbreviation, semester, startDate, endDate, minutes, note, SOK, TOK, ASS, LAB, ECTS, Status) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    // Bind the values to the query
    query.addBindValue(module.getLongName());
    query.addBindValue(module.getShortName());
    query.addBindValue(module.getSemester());
    query.addBindValue(module.getStartDate().toString("yyyy-MM-dd"));
    query.addBindValue(module.getEndDate().toString("yyyy-MM-dd"));
    query.addBindValue(module.getTimeMin());
    query.addBindValue(module.getNote());
    query.addBindValue(module.getSok());
    query.addBindValue(module.getTok());
    query.addBindValue(module.getAss());
    query.addBindValue(module.getLab());
    query.addBindValue(module.getEcts());
    query.addBindValue(module.getStatus());

    if (!query.exec()) {
        qDebug() << "Error: Unable to insert module into database";
        qDebug() << "Last error: " << query.lastError();
    }
}