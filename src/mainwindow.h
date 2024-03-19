#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "StudyTimer.h"
#include <QTableWidget>
#include <QTableView>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QStringList>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQueryModel>
#include <QDebug>
#include <QtGui/QColor>
#include "ColorDelegate.h"
#include "DbManager.h"
#include "DbInputWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

/**
 * @brief Constructor for the MainWindow class.
 * 
 * Initializes the MainWindow object and sets up various UI elements and connections.
 * 
 * @param parent Pointer to the parent widget.
 */
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Timer

    /**
 * @brief Updates the timer label with the given number of seconds.
 * 
 * Calculates hours, minutes, and seconds from the total seconds and updates the displayed timer label.
 * 
 * @param seconds The total number of seconds.
 */
    void updateTimerLabel(int seconds);

    
/**
 * @brief Updates the laps table with the given list of lap times.
 * 
 * Clears the laps table contents and populates it with the provided lap times.
 * 
 * @param laps The list of lap times.
 */

    void updateLapsTable(const QStringList &laps);

    // Here start the functions for the Tables !!!

    /**
 * @brief Opens the input window for adding modules.
 * 
 * Opens a dialog window for adding modules. After the window is closed, the main table is updated to reflect any changes.
 */
    void addModuleclicked();

    /**
 * @brief Opens the input window for editing a selected module.
 * 
 * Opens a dialog window populated with the selected module's data from the main table for editing. 
 * After the window is closed, the module is updated in the database and the main table is refreshed. 
 * (This function does not yet display the wanted behaviour)
 */
    void editModuleclicked();

    /**
 * @brief Deletes the selected module from the database.
 * 
 * Deletes the module corresponding to the selected row from the main table. If the deletion is successful, the main table is refreshed. 
 * (Does not work yet as expected, refreshing the table/database is faulty)
 */
    void deleteModuleclicked();
    //void updateMainTable();
    
    /**
 * @brief Handles the event when a row is clicked in the main table.
 * 
 * Retrieves details of the module corresponding to the clicked row and displays them in the detail table.
 * Also ensures the entire row (module) is selected when selecting a single field
 * 
 * @param current The model index of the currently clicked row.
 * @param previous The model index of the previously clicked row.
 */
    void onRowClicked(const QModelIndex &current, const QModelIndex &previous);

/**
 * @brief Updates the selected row index when the current row changes.
 * 
 * Updates the selectedRow variable with the index of the currently selected row in the main table.
 * 
 * @param current The model index of the currently selected row.
 * @param previous The model index of the previously selected row.
 */
    void onRowChanged(const QModelIndex &current, const QModelIndex &previous);

/**
 * @brief Displays details of a module in the detail table.
 * 
 * Retrieves and displays details of the module identified by the given abbreviation in the detail table.
 * 
 * @param abbreviation The abbreviation of the module to display details for.
 */
// Displaying details in detail table
    void ModuleDetailClicked(const QString &abbreviation);
    
private:
    Ui::MainWindow *ui;
    StudyTimer *studyTimer;
    QTableWidget *lapsTable;
    QTableView *MainTable;
    QTableView *DetailTable;
    QLabel *Title;
    DbManager *dbManager; // Add a pointer to a DbManager instance
    DbInputWindow *dbInputwindow;
    QSqlQueryModel *queryModel;
    int selectedRow;
};
#endif // MAINWINDOW_H
