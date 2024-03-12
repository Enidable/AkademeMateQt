#include "DbInputWindow.h"
#include "./ui_DbInputWindow.h"
#include <QMessageBox>
#include <QSqlQuery>


DbInputWindow::DbInputWindow(QWidget *parent) :
    QDialog(parent), ui(new Ui::DbInputWindow), dbManager()
{
    ui->setupUi(this);
    connect(ui->submitbutton, &QPushButton::clicked, this, &DbInputWindow::onSubmitButtonClicked);

    // Initialize private member variables
    xsok = false;
    xtok = false;
    xlab = false;
    xass = 0;

    // Open the database connection
    dbManager.openDatabaseConnection();
}

DbInputWindow::~DbInputWindow()
{
    delete ui;
}

void DbInputWindow::onSubmitButtonClicked()
{
    // Get input values from the fields
    QString short_name = ui->short_name_label->text();
    QString long_name = ui->Long_Name->text();
    int semester = ui->Semester_select->currentIndex() + 1; // +1 because the first item is "-"
    QString m_k_a = ui->M_K_A_select->currentText();
    QDate start_date = ui->Start_dateEdit->date();
    QDate end_date = ui->End_dateEdit->date();
    int time_min = ui->Time_Input->text().toInt();
    double note = ui->Note_Box->currentText().toDouble();
    int ects = ui->ECTS_Box->currentText().toInt();
    QString status = ui->status_box->currentText();
    QString m_k_a_2 = ui->M_K_A_select_2->currentText();

    // Parse performance assessment values
    xsok = m_k_a == "Sofort Online Klausur" || m_k_a_2 == "Sofort Online Klausur";
    xtok = m_k_a == "Termin Online Klausur" || m_k_a_2 == "Termin Online Klausur";
    xlab = m_k_a == "Laboratory report" || m_k_a_2 == "Laboratory report";
    xass = (m_k_a == "Assignment") + (m_k_a_2 == "Assignment");

    // Check if the database connection is open
    if (!dbManager.getDatabase().isOpen()) {
        qDebug() << "Error: Database connection not open.";
        return;
    }

    // Create a QSqlQuery object
    QSqlQuery query(dbManager.getDatabase());

    // Prepare the SQL INSERT statement
    QString sql = "INSERT INTO MainTable (Abbreviation, Module, Semester, Start, End, Minutes, Note, SOK, TOK, Assignment, LAB, ECTS, Status) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";

    // Bind the input values to the query parameters
    query.prepare(sql);
    query.addBindValue(short_name);
    query.addBindValue(long_name);
    query.addBindValue(semester);
    query.addBindValue(start_date);
    query.addBindValue(end_date);
    query.addBindValue(time_min);
    query.addBindValue(note);
    query.addBindValue(xsok);
    query.addBindValue(xtok);
    query.addBindValue(xass);
    query.addBindValue(xlab);
    query.addBindValue(ects);
    query.addBindValue(status);

    // Execute the query
    if (query.exec()) {
        qDebug() << "Module added successfully";
    } else {
        qDebug() << "Error adding module:" << query.lastError();
    }

    // Close input window
    close();
}
