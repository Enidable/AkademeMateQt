#ifndef DB_INPUT_WINDOW_H
#define DB_INPUT_WINDOW_H

#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>
#include <QtGui/QPalette>

QT_BEGIN_NAMESPACE
namespace Ui { class Db_Input_Window; }
QT_END_NAMESPACE

class Db_Input_Window : public QDb_Input_Window
{
    Q_OBJECT

public:
    Db_Input_Window(QWidget *parent = nullptr);
    ~Db_Input_Window();

private slots:

private:
    Ui::Db_Input_Window *ui;
};


#endif // DB_INPUT_WINDOW_H