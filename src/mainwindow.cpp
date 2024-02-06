#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "sqlite_helper.h"
#include "sqlite_result.h"


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

    lapsTable = ui->LapsTable;
     // Set individual column widths
    lapsTable->setColumnWidth(0, 30); 
    lapsTable->setColumnWidth(1, 60); 
}

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

void MainWindow::displayDatabaseInTable()
{
    // Connect to the SQLite database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("data/Module.db");

    if (!db.open()) {
        qDebug() << "Error: Unable to open database";
        return;
    }

    // Execute a SELECT query to retrieve data
    QSqlQuery query("SELECT * FROM Module_table");

    // Check if the query executed successfully
    if (!query.exec()) {
        qDebug() << "Error: Unable to execute query";
        db.close();
        return;
    }

    // Clear existing contents in MainTable
    ui->MainTable->clearContents();
    ui->MainTable->setRowCount(0);

    // Set column names dynamically based on the result set
    QSqlRecord record = query.record();
    int columnCount = record.count();
    QStringList columnNames;

    for (int col = 0; col < columnCount; ++col) {
        columnNames.append(record.fieldName(col));
    }

    ui->MainTable->setColumnCount(columnCount);
    ui->MainTable->setHorizontalHeaderLabels(columnNames);

    // Populate MainTable with data from the query
    int row = 0;
    while (query.next()) {
        ui->MainTable->insertRow(row);

        for (int col = 0; col < columnCount; ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            ui->MainTable->setItem(row, col, item);
        }

        ++row;
    }

    // Close the database connection
    db.close();
}