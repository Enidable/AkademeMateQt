#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), studyTimer(new StudyTimer(this)), dbInputwindow(new DbInputWindow(this))
{
    ui->setupUi(this);
    // Initialize the DbManager instance
    dbManager = DbManager::getInstance();

    // Open the database connection
    dbManager->openDatabaseConnection();
        // Display the database in the table view
    queryModel = dbManager->displayDatabaseInTable(ui->MainTable, dbManager->getDatabase());

    // Connect buttons to functions
    connect(ui->TStart_button, &QPushButton::clicked, studyTimer, &StudyTimer::start);
    connect(ui->TStop_button, &QPushButton::clicked, studyTimer, &StudyTimer::pause);
    connect(ui->TLap_button, &QPushButton::clicked, studyTimer, &StudyTimer::lap);
    connect(ui->TReset_button, &QPushButton::clicked, studyTimer, &StudyTimer::reset);

    // Connect StudyTimer signals to MainWindow slots
    connect(studyTimer, &StudyTimer::timerUpdated, this, &MainWindow::updateTimerLabel);
    connect(studyTimer, &StudyTimer::lapsUpdated, this, &MainWindow::updateLapsTable);

    // Buttons for adding/editing/deleting modules
    connect(ui->AddModuleButton, &QPushButton::clicked, this, &MainWindow::addModuleclicked);
    connect(ui->EditModuleButton, &QPushButton::clicked, this, &MainWindow::editModuleclicked);
    connect(ui->DeleteModuleButton, &QPushButton::clicked, this, &MainWindow::deleteModuleclicked);

    // Set the query model for the table view
    //queryModel = new QSqlQueryModel(this);
    //ui->MainTable->setModel(queryModel);
    // Connect the clicked signal to the onRowClicked slot
    connect(ui->MainTable->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainWindow::onRowClicked);

    lapsTable = ui->LapsTable;
    // Set individual column widths
    lapsTable->setColumnWidth(0, 30);
    lapsTable->setColumnWidth(1, 60);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete studyTimer;
    delete dbInputwindow;

    // Close the database connection
    dbManager->getDatabase().close();
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

//Open input window for editing and or adding modules
void MainWindow::addModuleclicked()
{
    DbInputWindow inputWindow;
    inputWindow.exec();
    dbManager->displayDatabaseInTable(ui->MainTable, dbManager->getDatabase());
}

void MainWindow::editModuleclicked()
{
    // Get the selected row
    int row = ui->MainTable->currentIndex().row();
/*
    // Get the module abbreviation from the selected row
    QString abbreviation = queryModel->data(queryModel->index(row, 1)).toString();

    // Query the database to retrieve the selected module
    Module selectedModule = dbManager->selectModule(abbreviation);

    // Display the selected module in the DbInputWindow and allow editing
    DbInputWindow inputWindow(this, true, selectedModule);
    inputWindow.exec();

    // Refresh the table view
    dbManager->displayDatabaseInTable(ui->MainTable, dbManager->getDatabase());
*/
}

void MainWindow::deleteModuleclicked()
{
    // Get the selected row
    int row = ui->MainTable->currentIndex().row();

    if (row != -1) // Ensure a valid row is selected
    {
        // Get the module abbreviation from the selected row
        QString abbreviation = queryModel->data(queryModel->index(row, 1)).toString();

        // Delete the selected module from the database
        if (dbManager->deleteModule(abbreviation, dbManager->getDatabase()))
        {
            // Refresh the table view
            dbManager->displayDatabaseInTable(ui->MainTable, dbManager->getDatabase());
        }
        else
        {
            QMessageBox::warning(this, "Error", "Failed to delete module from database.");
        }
    }
    else
    {
        QMessageBox::warning(this, "Error", "No module selected.");
    }
}

void MainWindow::onRowClicked(const QModelIndex &current, const QModelIndex &previous)
{
    // Ensure that the entire row is selected
    ui->MainTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Get the selected row
    int row = current.row();
    qDebug() << "current row" << row;

    // Get the module abbreviation from the selected row
    // Adjust the column number to match the column containing the abbreviation (column 1)
    QString abbreviation = queryModel->data(queryModel->index(row, 1)).toString();

    // Query the database to retrieve the selected module using the abbreviation
    Module selectedModule = dbManager->selectModule(abbreviation);
}
