#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), studyTimer(new StudyTimer(this))
{
    ui->setupUi(this);

    // Connect buttons to functions
    connect(ui->TStart_button, &QPushButton::clicked, studyTimer, &StudyTimer::start);
    connect(ui->TStop_button, &QPushButton::clicked, studyTimer, &StudyTimer::pause);
    connect(ui->TReset_button, &QPushButton::clicked, studyTimer, &StudyTimer::reset);
    connect(ui->TLap_button, &QPushButton::clicked, studyTimer, &StudyTimer::lap);

     // Connect StudyTimer signals to MainWindow slots
    connect(studyTimer, &StudyTimer::timerUpdated, this, &MainWindow::updateTimerLabel);
    connect(studyTimer, &StudyTimer::lapsUpdated, this, &MainWindow::updateLapsTable);

    lapsTable = ui->LapsTable;
     // Set individual column widths
    lapsTable->setColumnWidth(0, 30); 
    lapsTable->setColumnWidth(1, 60); 

    // MainTable creation and column settings
    MainTable = ui->MainTable;
    MainTable->setColumnWidth(0, 70);
    MainTable->setColumnWidth(1, 300);
    MainTable->setColumnWidth(2, 60);
    MainTable->setColumnWidth(3, 20);
    MainTable->setColumnWidth(4, 100);
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
