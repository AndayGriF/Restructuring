#include "startgame.h"
#include "ui_startgame.h"

StartGame::StartGame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StartGame)
{
    ui->setupUi(this);
}

StartGame::~StartGame()
{
    delete ui;
}

void StartGame::on_MainMenu_clicked()
{
    this->close();
    emit firstWindow();
}
