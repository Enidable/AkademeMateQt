#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), studyTimer(new StudyTimer(this))
{
    ui->setupUi(this);

    // Connect buttons to functions
    connect(ui->TStart_button, &QPushButton::clicked, studyTimer, &StudyTimer::start);
    connect(ui->TStop_button, &QPushButton::clicked, studyTimer, &StudyTimer::pause);
    connect(ui->TLap_button, &QPushButton::clicked, studyTimer, &StudyTimer::lap);
    connect(ui->TReset_button, &QPushButton::clicked, studyTimer, &StudyTimer::reset);

    // Connect StudyTimer signals to MainWindow slots
    connect(studyTimer, &StudyTimer::timerUpdated, this, &MainWindow::updateTimerLabel);
    connect(studyTimer, &StudyTimer::lapsUpdated, this, &MainWindow::updateLapsTable);

    // Connect Load Database button to function
    connect(ui->connect_db_button, &QPushButton::clicked, this, &MainWindow::openDatabaseConnection);
    connect(ui->Load_db_button, &QPushButton::clicked, this, &MainWindow::displayDatabaseInTable);

    lapsTable = ui->LapsTable;
    // Set individual column widths
    lapsTable->setColumnWidth(0, 30);
    lapsTable->setColumnWidth(1, 60);
}

/*
// This function can be removed (also remove declaration in mainwindow.h or we get a compile error)
void MainWindow::resetTimer() {
    // Clear the data in the existing lapsTable, keeping the column names intact
    for (int row = 0; row < lapsTable->rowCount(); ++row) {
        for (int col = 0; col < lapsTable->columnCount(); ++col) {
            QTableWidgetItem* item = lapsTable->item(row, col);
            if (item) {
                delete item;  // Delete the item to clear the content
                lapsTable->setItem(row, col, nullptr);  // Set the item to nullptr
            }
        }
    }
}
*/

MainWindow::~MainWindow()
{
    delete ui;
    delete studyTimer;
}

void MainWindow::updateTimerLabel(int seconds) {
    // Calculate hours, minutes, seconds
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    int remainingSeconds = seconds % 60;

// Format the time components as a string
    QString formattedTime = QString("%1:%2:%3")
                                .arg(hours, 2, 10, QLatin1Char('0'))
                                .arg(minutes, 2, 10, QLatin1Char('0'))
                                .arg(remainingSeconds, 2, 10, QLatin1Char('0'));

    // Update the displayed timer label
    ui->labelTimer->setText(formattedTime);}

void MainWindow::updateLapsTable(const QStringList &laps) {
    lapsTable->clearContents();
    lapsTable->setRowCount(laps.size());

    for (int i = 0; i < laps.size(); ++i) {
        // Assuming laps[i] is already formatted as "HH:MM:SS"
        QString formattedTime = laps[i];

        // Create a QTableWidgetItem with the formatted time
        QTableWidgetItem *lapNumberItem = new QTableWidgetItem(QString::number(i + 1)); // Lap number
        QTableWidgetItem *lapTimeItem = new QTableWidgetItem(formattedTime); // Lap time

        // Set lap number in the first column (column 0)
        lapsTable->setItem(i, 0, lapNumberItem);

        // Set lap time in the second column (column 1)
        lapsTable->setItem(i, 1, lapTimeItem);
    }
}

void MainWindow::openDatabaseConnection() {
    // Check if the database connection is already open
    if (database.isOpen()) {
        return;
    }
    // Add the SQLite driver
    database = QSqlDatabase::addDatabase("QSQLITE");
    // Set the database name
    // executable will be in /build, so use a relative path from there to find find db in directory data
    database.setDatabaseName("../data/Module.db");
    // Open the database
    if (!database.open()) {
        qDebug() << "Error: Unable to open database connection";
                return;
    }else {
        qDebug() << "Database connection is open";
        QMessageBox::information(this, "Success", "Database connection is open");
        return;
    }
}

void MainWindow::displayDatabaseInTable() {
    // Create a QSqlQueryModel to hold your data
    QSqlQueryModel *model = new QSqlQueryModel();
    
    QString queryStr = "SELECT * FROM \"Studium Zeitmanagement - db (1)\"";
    
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
    
    // Create a QTableView
    QTableView *tableView = new QTableView();
    
    // Set the model for MainTable
    MainTable->setModel(model);
    
    setCentralWidget(tableView);

    tableView->resizeColumnsToContents();
}