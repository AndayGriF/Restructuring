#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "random.h"

typedef struct player
{
    int money;
    int count_card;
} player;

typedef struct playerPK1
{
    int money;
    int count_card;
} playerPK1;

int cards[15]; //Колода карт

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

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWindow)
{    
    ui->setupUi(this);
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::on_changeButton_clicked()
{
    fillCards(&cards[0]);
    QString s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14;
    ui->checkLabel->setText(s0.setNum(cards[0]) + " " + s1.setNum(cards[1]) + " " + s2.setNum(cards[2]) + " " + s3.setNum(cards[3]) + " " + s4.setNum(cards[4]) + "\n" +
                            s5.setNum(cards[5]) + " " + s6.setNum(cards[6]) + " " + s7.setNum(cards[7]) + " " + s8.setNum(cards[8]) + " " + s9.setNum(cards[9]) + "\n" +
                            s10.setNum(cards[10]) + " " + s11.setNum(cards[11]) + " " + s12.setNum(cards[12]) + " " + s13.setNum(cards[13]) + " " + s14.setNum(cards[14]));


    ui->card1Button->setText(cardFromIntToStr(cards[0]));
    ui->card2Button->setText(cardFromIntToStr(cards[1]));
}
