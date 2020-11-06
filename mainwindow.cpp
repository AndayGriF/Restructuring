#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPixmap>

//"C:/Users/Andrey/Document/Programing/Resource/main.jpg

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap imageMain (":/Recource/Image/main.jpg");
    ui->ImageMM -> setPixmap (imageMain);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_StartButton_clicked()
{
    hide();
    SG = new StartGame(this);
    SG->show();
}

void MainWindow::on_SettingsButton_clicked()
{
    hide();
    SET = new Settings(this);
    SET->show();
}
