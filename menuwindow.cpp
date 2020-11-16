#include "menuwindow.h"
#include "./ui_menuwindow.h"

MenuWindow::MenuWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MenuWindow)
{
    ui->setupUi(this);
    FlatWin = new FlatWindow();
    connect(FlatWin, &FlatWindow::firstwindow, this, &MenuWindow::show);
    SetWin = new SettingsWindow();
    connect(SetWin, &SettingsWindow::firstwindow, this, &MenuWindow::show);
}

MenuWindow::~MenuWindow()
{
    delete ui;
}


void MenuWindow::on_startButton_clicked()
{
    FlatWin->show();
    this->close();
}

void MenuWindow::on_settingsButton_clicked()
{
    SetWin->show();
    this->close();
}

void MenuWindow::on_exitButton_clicked()
{
    this->close();
}
