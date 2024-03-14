#include "DbInputWindow.h"
#include "./ui_DbInputWindow.h"

DbInputWindow::DbInputWindow(QWidget *parent) :
    QDialog(parent), ui(new Ui::DbInputWindow), dbManager(new DbManager(this))
{
    ui->setupUi(this);
    connect(ui->submitbutton, &QPushButton::clicked, this, &DbInputWindow::onSubmitButtonClicked);

    // Initialize private member variables
    xsok = false;
    xtok = false;
    xlab = false;
    xass = 0;

    // Open the database connection
    dbManager->openDatabaseConnection();
}

DbInputWindow::~DbInputWindow()
{
    delete ui;
    delete dbManager;
}

void DbInputWindow::onSubmitButtonClicked()
{
    // Get input values from the fields
    QString short_name = ui->short_name_label->text();
    qDebug() << "Short name:" << short_name;

    QString long_name = ui->Long_Name->text();
    qDebug() << "Long name:" << long_name;

    if (long_name.isEmpty()) {
        QMessageBox::warning(this, "Error", "Name cannot be empty.");
        return;
    }

    int semester = ui->Semester_select->currentIndex() + 1; // +1 because the first item is "-"
    qDebug() << "Semester:" << semester;

    QString m_k_a = ui->M_K_A_select->currentText();
    qDebug() << "M_K_A:" << m_k_a;

    QDate start_date = ui->Start_dateEdit->date();
    qDebug() << "Start date:" << start_date;

    QDate end_date = ui->End_dateEdit->date();
    qDebug() << "End date:" << end_date;

    int time_min = ui->Time_Input->text().toInt();
    qDebug() << "Time (min):" << time_min;

    double note = ui->Note_Box->currentText().toDouble();
    qDebug() << "Note:" << note;

    int ects = ui->ECTS_Box->currentText().toInt();
    qDebug() << "ECTS:" << ects;

    QString status = ui->status_box->currentText();
    qDebug() << "Status:" << status;

    QString m_k_a_2 = ui->M_K_A_select_2->currentText();
    qDebug() << "M_K_A_2:" << m_k_a_2;

    // Parse performance assessment values
    xsok = m_k_a == "Sofort Online Klausur" || m_k_a_2 == "Sofort Online Klausur";
    qDebug() << "SOK:" << xsok;

    xtok = m_k_a == "Termin Online Klausur" || m_k_a_2 == "Termin Online Klausur";
    qDebug() << "TOK:" << xtok;

    xlab = m_k_a == "Laboratory report" || m_k_a_2 == "Laboratory report";
    qDebug() << "LAB:" << xlab;

    xass = (m_k_a == "Assignment") + (m_k_a_2 == "Assignment");
    qDebug() << "ASS:" << xass;

    // Create a Module object
    Module module(short_name, long_name, semester, m_k_a, start_date, end_date, time_min, note, ects, xsok, xtok, xass, xlab, status);

    // Add the module to the database
    dbManager->insertModule(module, dbManager->getDatabase());

    // Close input window
    close();
}