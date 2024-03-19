#include "DbInputWindow.h"
#include "./ui_DbInputWindow.h"

DbInputWindow::DbInputWindow(QWidget *parent, bool isEdit) :
    QDialog(parent), ui(new Ui::DbInputWindow)
{
    ui->setupUi(this);
    this->isEdit = isEdit;
    connect(ui->submitbutton, &QPushButton::clicked, this, &DbInputWindow::onSubmitButtonClicked);

    // Set default values for start_date and end_date
    QDate currentDate = QDate::currentDate();

    // Set calendarPopup attribute to true and set an empty QDate as default value
    ui->Start_dateEdit->setCalendarPopup(true);
    ui->Start_dateEdit->setDate(QDate(currentDate));
    ui->Start_dateEdit->setDisplayFormat("dd.MM.yyyy ' '"); // Set custom format for date

    ui->End_dateEdit->setCalendarPopup(true);
    ui->End_dateEdit->setDate(currentDate.addDays(42));
    ui->End_dateEdit->setDisplayFormat("dd.MM.yyyy ' '"); // Set custom format for date

    // Initialize private member variables
    xsok = false;
    xtok = false;
    xlab = false;
    xass = 0;

    // Open the database connection
    dbManager = DbManager::getInstance();
    dbManager->openDatabaseConnection();
}

DbInputWindow::~DbInputWindow()
{
    delete ui;
}

int getStatusID(const QString &status)
{
    if (status == "inactive")
        return 1;
    else if (status == "active")
        return 2;
    else if (status == "completed")
        return 3;
    else
        return 1; // default ststus is inactive
}

QString getStatusText(int statusID)
{
    switch (statusID)
    {
        case 1:
            return "inactive";
        case 2:
            return "active";
        case 3:
            return "completed";
        default:
            return "inactive"; // Set default to "Inactive"
    }
}


void DbInputWindow::onSubmitButtonClicked()
{
    // Get input values from the fields
    QString long_name = ui->Long_Name->text();
    qDebug() << "Long name:" << long_name;    

      if (long_name.isEmpty()) {
        QMessageBox::warning(this, "Error", "Name cannot be empty.");
        return;
    }  
   
    QString short_name = ui->short_name_label->text();
    qDebug() << "Short name:" << short_name;

    int semester = ui->Semester_select->currentIndex(); // +1 because the first item is "-"
    qDebug() << "Semester:" << semester;

    QDate start_date = ui->Start_dateEdit->date();
    qDebug() << "Start date:" << start_date;

    QDate end_date = ui->End_dateEdit->date();
    qDebug() << "End date:" << end_date;

        int time_min = ui->Time_Input->text().toInt();
    qDebug() << "Time (min):" << time_min;
    
    double note = ui->Note_Box->currentText().toDouble();
    qDebug() << "Note:" << note;
    
    QString m_k_a = ui->M_K_A_select->currentText();
    qDebug() << "M_K_A:" << m_k_a;

    QString m_k_a_2 = ui->M_K_A_select_2->currentText();
    qDebug() << "M_K_A_2:" << m_k_a_2;

    int ects = ui->ECTS_Box->currentText().toInt();
    qDebug() << "ECTS:" << ects;

    QString status = ui->status_box->currentText();
    qDebug() << "Status:" << status;

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
    Module module(short_name, long_name, semester, start_date, end_date, time_min, note, ects, xsok, xtok, xass, xlab, getStatusID (status));
    
   // Check if the input window is for editing a module
    if (isEdit)
    {
        // Update the module in the database
        dbManager->updateModule(module, dbManager->getDatabase());
    }
    else
    {
        // Add the module to the database
        dbManager->insertModule(module, dbManager->getDatabase());
    }

    // Close input window
    close();
}

void DbInputWindow::setModule(const Module &module)
{
    // this->module = module;
    ui->Long_Name->setText(module.getLongName());
    ui->short_name_label->setText(module.getShortName());
    ui->Semester_select->setCurrentIndex(module.getSemester() - 1);
    ui->Start_dateEdit->setDate(module.getStartDate());
    ui->End_dateEdit->setDate(module.getEndDate());
    ui->Time_Input->setText(QString::number(module.getTimeMin()));
    QString noteString = QString::number(module.getNote(), 'f', 2);
    ui->Note_Box->setCurrentText(noteString);
    ui->M_K_A_select->setCurrentText(module.getSok() ? "Sofort Online Klausur" : module.getTok() ? "Termin Online Klausur" : "");
    ui->M_K_A_select_2->setCurrentText(module.getAss() > 0 ? "Assignment" : module.getLab() ? "Laboratory report" : "");
    ui->ECTS_Box->setCurrentText(QString::number(module.getEcts()));
    ui->status_box->setCurrentText(getStatusText(module.getStatusID()));

    isEdit = true;
}