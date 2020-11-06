#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap imageMain (":/Recource/Image/main.jpg");
    ui->ImageMM -> setPixmap (imageMain);
    SG = new StartGame(this);
    connect(SG, &StartGame::firstWindow, this, &MainWindow::show);
    SET = new Settings(this);
    connect(SET, &Settings::firstWindow, this, &MainWindow::show);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_StartButton_clicked()
{
    hide();
    SG->show();
}

void MainWindow::on_SettingsButton_clicked()
{
    hide();
    SET->show();
}
