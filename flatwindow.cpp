#include "flatwindow.h"
#include "ui_flatwindow.h"

FlatWindow::FlatWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FlatWindow)
{
    ui->setupUi(this);
}

FlatWindow::~FlatWindow()
{
    delete ui;
}

void FlatWindow::on_backmenuBatton_clicked()
{
    this->close();
    emit firstwindow();
}
