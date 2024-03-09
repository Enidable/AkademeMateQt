#include "Db_input_Window.h"
#include "Db_Input_Window./ui_"

Db_Input_Window::Db_Input_Window(QWidget *parent) : QDb_Input_Window(parent), ui(new Ui::Db_Input_Window)
{
    ui->setupUi(this);

    
}