#include "flatwindow.h"
#include "ui_flatwindow.h"

int countPlayer = 0;

FlatWindow::FlatWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FlatWindow)
{
    ui->setupUi(this);
    ui->listPlayer->addItem("UserPlayer");
    ui->startGameButton->setEnabled(false);
    GameWin = new GameWindow();
    connect(GameWin,&GameWindow::firstwindow, this, &FlatWindow::show);
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

void FlatWindow::on_addPlayerButton_clicked()
{
        QString str , tr;
        if (countPlayer < 4)
        {
            if (ui->listPlayer->count() == 1)
            {
                ui->listPlayer->addItem("PlayerPc");
                countPlayer++;
            }
            else ui->listPlayer->addItem("PlayerPc" + str.setNum(countPlayer++) +1);
        }
        if (ui->listPlayer->count() > 1) ui->startGameButton->setEnabled(true);
}

void FlatWindow::on_deletePlayerButton_clicked()
{
    QString str ,tr;
    if (ui->listPlayer->count() > 1)
    {
        delete ui->listPlayer->item(ui->listPlayer->count() -1);
        countPlayer--;
    }
    if (ui->listPlayer->count() < 2) ui->startGameButton->setEnabled(false);
}

void FlatWindow::on_startGameButton_clicked()
{
    if (ui->listPlayer->count() > 1 && ui->listPlayer->count() < 3)
    {
        QMessageBox questBox;
        questBox.setWindowTitle("Внимание");
        questBox.setText("Для более приятной игры\nрекомендованно использовать 3 и более игроков");
        questBox.setIcon(QMessageBox::Question);
        QPushButton *next = questBox.addButton(tr("Продолжить"), QMessageBox::ActionRole);
        QPushButton *cancel = questBox.addButton(tr("Отмена"), QMessageBox::ActionRole);
        questBox.exec();
        if (questBox.clickedButton()== next)
        {
            GameWin->show();
            this->close();
        }
        else return;
    }
    GameWin->show();
    this->close();
}
