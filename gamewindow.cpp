#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "random.h"

static int cards[15]; //Колода карт
QString typeCardPlayer;
QString typeCardPC;


typedef struct
{
    int money;
    int count_card;
} Player;

void initPlayer(Player *player)
{
    player->money = 2;
    player->count_card = 2;
}

Player *player = (Player*) malloc(sizeof(Player));
Player *playerPC = (Player*) malloc(sizeof(Player));

void fillCards(int *cards)  //Заполнение колоды
{
    int endPred = 0;   //Предприниматель [0]
    int killer = 0;    //Киллер          [1]
    int presntTV = 0;  //Телеведущий     [2]
    int burocrat = 0;  //Бюрократ        [3]
    int dissid = 0;    //Диссидент       [4]
    int num;

    DefRandom();
    int i = 0;
    do
    {
       num = Random(0, 4);
       if (num == 0 && endPred != 3)
       {
           endPred++;
           *(cards + i) = num;
           i++;
           continue;
       }
       else if (num == 1 && killer != 3)
       {
           killer++;
           *(cards + i) = num;
           i++;
           continue;
       }
       else if (num == 2 && burocrat != 3)
       {
           burocrat++;
           *(cards + i) = num;
           i++;
           continue;
       }
       else if (num == 3 && presntTV != 3)
       {
           presntTV++;
           *(cards + i) = num;
           i++;
           continue;
       }
       else if (num == 4 && dissid != 3)
       {
           dissid++;
           *(cards + i) = num;
           i++;
           continue;
       }
    } while(i <= 14);
}

QString cardFromIntToStr(int cards) //Преобразование индекса для корректного отображения названия карты
{
    if (cards == 0) return "Предприниматель";
    if (cards == 1) return "Киллер";
    if (cards == 2) return "Телеведущий";
    if (cards == 3) return "Бюрократ";
    if (cards == 4) return "Диссидент";
    return "Нет такой карты";
}

void cardMoney(QString str, Player *player) //Прибавить или отнять монеты в зависимости от карты
{
    if(str == "Предприниматель") player->money += 3;
    else if(str == "Киллер") player->money -= 4;
    else if(str == "Телеведущий") player->money -= 1;
    else if(str == "Бюрократ") player->money += 2;
    else if(str == "Диссидент") player->money += 5;
}

QString cardColor(int cards)
{
    if (cards == 0) return "border-style: solid; border-width: 3px; border-color: green; border-radius: 10px;";
    if (cards == 1) return "border-style: solid; border-width: 3px; border-color: blue; border-radius: 10px;";
    if (cards == 2) return "border-style: solid; border-width: 3px; border-color: yellow; border-radius: 10px;";
    if (cards == 3) return "border-style: solid; border-width: 3px; border-color: red; border-radius: 10px;";
    if (cards == 4) return "border-style: solid; border-width: 3px; border-color: red; border-radius: 10px;";
    return "";
}

bool checkingPC()
{
    int check1 = 0, check2 = 0;
    check1 = Random(0, 1);
    check2 = Random(0, 1);
    if (check1 + check2 == 2) return true;
    return false;
}

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWindow)
{
    initPlayer(player);
    initPlayer(playerPC);
    fillCards(&cards[0]);
    ui->setupUi(this);
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::on_changeButton_clicked()
{
    ui->statusPCText->setText("");
    fillCards(&cards[0]);

    ui->checkLabel->setStyleSheet("border-style: solid;"
                                  "border-width: 1px;"
                                  "border-color: black;");

    ui->card1PCLabel->setStyleSheet("border-style: solid;"
                                    "border-width: 3px;"
                                    "border-color: black;"
                                    "border-radius: 10px;");

    ui->card2PCLabel->setStyleSheet("border-style: solid;"
                                    "border-width: 3px;"
                                    "border-color: black;"
                                    "border-radius: 10px;");

    QString s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, color;
    ui->checkLabel->setText(s0.setNum(cards[0]) + " " + s1.setNum(cards[1]) + " " + s2.setNum(cards[2]) + " " + s3.setNum(cards[3]) + " " + s4.setNum(cards[4]) + "\n" +
                            s5.setNum(cards[5]) + " " + s6.setNum(cards[6]) + " " + s7.setNum(cards[7]) + " " + s8.setNum(cards[8]) + " " + s9.setNum(cards[9]) + "\n" +
                            s10.setNum(cards[10]) + " " + s11.setNum(cards[11]) + " " + s12.setNum(cards[12]) + " " + s13.setNum(cards[13]) + " " + s14.setNum(cards[14]));

    ui->card1Button->setText(cardFromIntToStr(cards[0]));
    ui->card1Button->setStyleSheet(cardColor(cards[0]));
    ui->card2Button->setText(cardFromIntToStr(cards[1]));
    ui->card2Button->setStyleSheet(cardColor(cards[1]));

    ui->card1PCLabel->setText(cardFromIntToStr(cards[2]));
    //ui->card1PCLabel->setStyleSheet(cardColor(cards[2]));
    ui->card2PCLabel->setText(cardFromIntToStr(cards[3]));
    //ui->card2PCLabel->setStyleSheet(cardColor(cards[3]));
}

void GameWindow::on_card1Button_clicked()
{
    typeCardPlayer = ui->card1Button->text();
    QString str;
    if ((typeCardPlayer == "Киллер") && (player->money < 4))
    {
        QMessageBox::critical(this, "Предупреждение", "У вас недостаточно монет!\nДля использования Киллера необходимо заплатить 4 монеты!");
        return;
    }
    if ((typeCardPlayer == "Телеведущий") && (player->money < 1))
    {
        QMessageBox::critical(this, "Предупреждение", "У вас недостаточно монет!\nДля использования Телеведущего необходимо заплатить 1 монету!");
        return;
    }
    if (typeCardPlayer == "Диссидент")
    {
        QMessageBox::critical(this, "Предупреждение", "Нельзя ходить картой реакции!");
        return;
    }
    if ((typeCardPlayer == "Бюрократ") && (playerPC->money < 2))
    {
        QMessageBox::critical(this, "Предупреждение", "У соперника недостаточно монет!");
        return;
    }

    if (checkingPC())
    {
        ui->statusPCText->setText("ПРОВЕРЯЮ!");
    }
    else
    {
        cardMoney(typeCardPlayer, player);
        ui->moneyLabel->setText(str.setNum(player->money));
    }
}

void GameWindow::on_card2Button_clicked()
{
    typeCardPlayer = ui->card2Button->text();
    QString str;
    if ((typeCardPlayer == "Киллер") && (player->money < 4))
    {
        QMessageBox::critical(this, "Предупреждение", "У вас недостаточно монет!\nДля использования Киллера необходимо заплатить 4 монеты!");
        return;
    }
    if ((typeCardPlayer == "Телеведущий") && (player->money < 1))
    {
         QMessageBox::critical(this, "Предупреждение", "У вас недостаточно монет!\nДля использования Телеведущего необходимо заплатить 1 монету!");
        return;
    }
    if (typeCardPlayer == "Диссидент")
    {
        QMessageBox::critical(this, "Предупреждение", "Нельзя ходить картой реакции!");
        return;
    }
    if ((typeCardPlayer == "Бюрократ") && (playerPC->money < 2))
    {
        QMessageBox::critical(this, "Предупреждение", "У соперника недостаточно монет!");
        return;
    }

    if (checkingPC())
    {
        ui->statusPCText->setText("ПРОВЕРЯЮ!");
    }
    else
    {
        cardMoney(typeCardPlayer, player);
        ui->moneyLabel->setText(str.setNum(player->money));
    }
}
