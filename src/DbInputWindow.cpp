#include "DbInputWindow.h"
#include "./ui_DbInputWindow.h"
#include <QMessageBox>
#include <QSqlQuery>


::DbInputWindow::DbInputWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DbInputWindow)
{
    ui->setupUi(this);
    connect(ui->submitbutton, &QPushButton::clicked, this, &DbInputWindow::onSubmitButtonClicked);

    // Initialize private member variables
    xsok = false;
    xtok = false;
    xlab = false;
    xass = 0;
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

/*

    // Add input values to the database table
     qDebug() << "Insert into database:" << short_name << long_name << semester << xsok << xtok << xlab << xass << start_date << end_date << time_min << note << ects << status;

    // Execute the query
    if (query.exec()) {
        qDebug() << "Module added successfully";
    } else {
        qDebug() << "Error adding module:" << query.lastError();
    }
*/
    // Close input window
    close();
}