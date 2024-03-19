#ifndef DBINPUTWINDOW_H
#define DBINPUTWINDOW_H

#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>
#include <QtGui/QPalette>
#include <QComboBox>
#include <QDialog>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include "DbManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DbInputWindow; }
QT_END_NAMESPACE

/**
 * @brief The DbInputWindow class represents a dialog window for inputting (and editing) modules into the database.
 */

class DbInputWindow : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a DbInputWindow object.
     * 
     * @param parent Optional parent widget.
     * @param isEdit Flag indicating whether the window is for editing an existing module.
     */
    explicit DbInputWindow(QWidget *parent = nullptr, bool isEdit = false);
   
    /**
     * @brief Destroys the DbInputWindow object.
     */
    ~DbInputWindow();

    /**
     * @brief Sets the data of the selected module for editing to be displayed in the input window.
     * 
     * @param module The module to be displayed.
     */
    void setModule(const Module &module);

    /**
     * @brief Retrieves the module data entered in the input window.
     * Something about editing modules is still wrong, causing the modules not to be updated in the table
     * @return The module entered in the window.
     */
    Module getModule() const;

private slots:
    /**
     * @brief Slot for handling the submission button clicked event.
     * This button takes all the information that was filled into the input window and writes it into the database
     */
    /**
     * @brief Handles the event when the submit button is clicked in the database input window.
     * 
     * This function retrieves input values from the fields in the database input window. 
     * It then creates a Module object with the retrieved data and either updates the existing module in the database if the window is in edit mode,
     * or inserts a new module into the database if it's in add mode and closes the input window.
     * 
     * @note This function assumes that all necessary input fields in the database input window are correctly filled out.
     */
    void onSubmitButtonClicked();

private:
    Ui::DbInputWindow *ui; /**< Pointer to the user interface object. */
    DbManager *dbManager; /**< Pointer to the database manager object. */
    bool xsok; /**< Flag indicating presence of immediate online exam. */
    bool xtok; /**< Flag indicating presence of scheduled online exam. */
    bool xlab; /**< Flag indicating presence of laboratory report. */
    int xass; /**< Number of assignments. */
    bool isEdit; /**< Flag indicating whether the window is for editing an existing module. */
};


#endif // DBINPUTWINDOW_H