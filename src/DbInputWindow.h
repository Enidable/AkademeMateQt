#ifndef DBINPUTWINDOW_H
#define DBINPUTWINDOW_H

#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>
#include <QtGui/QPalette>
#include <QComboBox>
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class DbInputWindow; }
QT_END_NAMESPACE

class DbInputWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DbInputWindow(QWidget *parent = nullptr);
    ~DbInputWindow();

private slots:
    void onSubmitButtonClicked();

private:
    Ui::DbInputWindow *ui;
    bool xsok;
    bool xtok;
    bool xlab;
    int xass;
};


#endif // DBINPUTWINDOW_H