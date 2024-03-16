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
QSqlDatabase &DbManager::getDatabase()
{
    return database;
}

void DbManager::initializeDatabase(QSqlDatabase &database)
{
    // Create tables / data structure
    QSqlQuery query(database);
    if (!query.exec("CREATE TABLE IF NOT EXISTS Module ("
                    "ModuleID INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "Module TEXT,"
                    "Abbreviation TEXT KEY,"
                    "Semester INTEGER,"
                    "Start DATE,"
                    "End DATE,"
                    "Minutes INTEGER,"
                    "Note TEXT,"
                    "SOK BOOLEAN,"
                    "TOK BOOLEAN,"
                    "ASS INTEGER,"
                    "LAB BOOLEAN,"
                    "ECTS INTEGER,"
                    "Status TEXT,"
                    "UNIQUE (Abbreviation))"))
    {
        qDebug() << "Failed to create Module table: " << query.lastError();
    }
    else
    {
        qDebug() << "Module table created or already exists.";
    }

    if (!query.exec("CREATE TABLE IF NOT EXISTS Assignment_Laboratory_report ("
                    "AssID INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "ModuleID INTEGER,"
                    "AssName TEXT,"
                    "Type TEXT,"
                    "Status TEXT,"
                    "FOREIGN KEY (ModuleID) REFERENCES Module (ModuleID))"))
    {
        qDebug() << "Failed to create Assignment_Laboratory_report table: " << query.lastError();
    }
    else
    {
        qDebug() << "Assignment_Laboratory_report table created or already exists.";
    }

    if (!query.exec("CREATE TABLE IF NOT EXISTS Exam ("
                    "ExamID INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "ModuleID INTEGER,"
                    "ExamName TEXT,"
                    "Status TEXT,"
                    "FOREIGN KEY (ModuleID) REFERENCES Module (ModuleID))"))
    {
        qDebug() << "Failed to create Exam table: " << query.lastError();
    }
    else
    {
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
                    "FOREIGN KEY (AssID) REFERENCES Assignment_Laboratory_report (AssID))"))
    {
        qDebug() << "Failed to create Time_ASS table: " << query.lastError();
    }
    else
    {
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
                    "FOREIGN KEY (ExamID) REFERENCES Exam (ExamID))"))
    {
        qDebug() << "Failed to create Time_Exam table: " << query.lastError();
    }
    else
    {
        qDebug() << "Time_Exam table created or already exists.";
    }
}

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
    const QString bulletPoint = "◆";    // make this configurable

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
        Status
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


void DbManager::insertModule(const Module &module, QSqlDatabase &database)
{
    QSqlQuery query(database); // Pass the database object to the QSqlQuery constructor

    if (!database.isOpen())
    {
        qDebug() << "Error: Database not open";
        return;
    }

    query.prepare("INSERT INTO Module (Module, Abbreviation, Semester, Start, End, Minutes, Note, SOK, TOK, ASS, LAB, ECTS, Status) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
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
    query.addBindValue(module.getStatus());

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
    // Make sure to update the following lines with the correct column names and data types
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
    QString status = query.value("Status").toString();

    return Module(shortName, longName, semester, startDate, endDate, timeMin, note, ects, sok, tok, ass, lab, status);
}