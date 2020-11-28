#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "random.h"

static int cards[15]; //Колода карт
QString typeCardPlayer;
int statusPC = 0;     //0 - ожидание или ход, 1 - проверка или нападение
QString lastUseCard;

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

void GameWindow::cardMoney(QString typeCard, Player *playerFrom, Player *playerTo)
{
    if(typeCard == "Предприниматель")
    {
        playerTo->money += 3;
        return;
    }
    if(typeCard == "Киллер")
    {
        playerTo->money -= 4;
        return;
    }
    if(typeCard == "Телеведущий")
    {
        playerTo->money -= 1;
        return;
    }
    if(typeCard == "Бюрократ")
    {
        if (playerFrom->money >= 2)
        {
            playerFrom->money -= 2;
            playerTo->money +=2;
            return;
        }
        if (playerFrom->money == 1)
        {
            playerFrom->money -= 1;
            playerTo->money += 1;
            return;
        }
    }
    if(typeCard == "Диссидент")
    {
        playerTo->money += 5;
        return;
    }
}

void GameWindow::enabledButtons(bool b)
{
    ui->greenButton->setEnabled(b);
    ui->yellowButton->setEnabled(b);
    ui->blueButton->setEnabled(b);
    ui->redButton->setEnabled(b);
    ui->checkButton->setEnabled(b);
    ui->money1Button->setEnabled(b);
    ui->restructButton->setEnabled(b);
}

void GameWindow::enabledAct()
{
    if(statusPC == 1 && player->count_card == 2)
    {
        ui->greenButton->setEnabled(false);
        ui->yellowButton->setEnabled(false);
        ui->blueButton->setEnabled(false);
        ui->redButton->setEnabled(false);
        ui->reactButton->setEnabled(true);
        ui->money1Button->setEnabled(false);
        ui->restructButton->setEnabled(false);
        ui->checkButton->setEnabled(false);
        return;
    }
    if(statusPC == 1 && player->count_card == 1)
    {
        ui->greenButton->setEnabled(false);
        ui->yellowButton->setEnabled(false);
        ui->blueButton->setEnabled(false);
        ui->redButton->setEnabled(false);
        ui->reactButton->setEnabled(false);
        ui->money1Button->setEnabled(false);
        ui->restructButton->setEnabled(false);
        ui->checkButton->setEnabled(false);
        return;
    }
    if(player->money < 1)
    {
        ui->greenButton->setEnabled(true);
        ui->yellowButton->setEnabled(false);
        ui->blueButton->setEnabled(false);
        ui->redButton->setEnabled(true);
        ui->reactButton->setEnabled(false);
        ui->money1Button->setEnabled(true);
        ui->restructButton->setEnabled(false);
        ui->checkButton->setEnabled(false);
        return;
    }
    if(player->money < 4)
    {
        ui->greenButton->setEnabled(true);
        ui->yellowButton->setEnabled(true);
        ui->blueButton->setEnabled(false);
        ui->redButton->setEnabled(true);
        ui->reactButton->setEnabled(false);
        ui->money1Button->setEnabled(true);
        ui->restructButton->setEnabled(false);
        ui->checkButton->setEnabled(false);
        return;
    }
    if (player->money < 7)
    {
        ui->greenButton->setEnabled(true);
        ui->yellowButton->setEnabled(true);
        ui->blueButton->setEnabled(true);
        ui->redButton->setEnabled(true);
        ui->reactButton->setEnabled(false);
        ui->money1Button->setEnabled(true);
        ui->restructButton->setEnabled(false);
        ui->checkButton->setEnabled(false);
        return;
    }
    if (player->money < 10)
    {
        ui->greenButton->setEnabled(true);
        ui->yellowButton->setEnabled(true);
        ui->blueButton->setEnabled(true);
        ui->redButton->setEnabled(true);
        ui->reactButton->setEnabled(false);
        ui->restructButton->setEnabled(true);
        ui->checkButton->setEnabled(false);
        return;
    }
    if (player->money >= 10)
    {
        ui->greenButton->setEnabled(true);
        ui->yellowButton->setEnabled(true);
        ui->blueButton->setEnabled(true);
        ui->redButton->setEnabled(true);
        ui->reactButton->setEnabled(false);
        ui->money1Button->setEnabled(true);
        ui->restructButton->setEnabled(true);
        ui->checkButton->setEnabled(false);
//        ui->greenButton->setEnabled(false);
//        ui->yellowButton->setEnabled(false);
//        ui->blueButton->setEnabled(false);
//        ui->redButton->setEnabled(false);
//        ui->reactButton->setEnabled(false);
//        ui->money1Button->setEnabled(false);
//        ui->restructButton->setEnabled(true);
//        ui->checkButton->setEnabled(false);
        return;
    }
}

void GameWindow::dropCardPC()
{
    if (playerPC->count_card == 2)
    {
        QString typeCardPC1 = ui->card1PCLabel->text();
        QString typeCardPC2 = ui->card2PCLabel->text();
        QString str;
        if (typeCardPC1 == "Диссидент")
        {
            lastUseCard = "typeCardPC1";
            playerPC->count_card--;
            cardMoney(typeCardPC1, player, playerPC);
            ui->moneyPCLabel->setText(str.setNum(playerPC->money));
            ui->card1PCLabel->setGeometry(318,70,131,141);
            ui->card1PCLabel->setEnabled(false);
            ui->deathCardPC1Label->setText("Мертвый\nДиссидент");
            return;
        }
        if (typeCardPC2 == "Диссидент")
        {
            lastUseCard = typeCardPC2;
            playerPC->count_card--;
            cardMoney(typeCardPC2, player, playerPC);
            ui->moneyPCLabel->setText(str.setNum(playerPC->money));
            ui->card2PCLabel->setGeometry(455,70,131,141);
            ui->card2PCLabel->setEnabled(false);
            ui->deathCardPC2Label->setText("Мертвый\nДиссидент");
            return;
        }
        DefRandom();
        int isDiss = Random(0,1);
        int isCard = Random(1,2);
        if (isDiss)
        {
            if (isCard == 1)
            {
                lastUseCard = "Диссидент";
                playerPC->count_card--;
                cardMoney(lastUseCard, player, playerPC);
                ui->moneyPCLabel->setText(str.setNum(playerPC->money));
                ui->card1PCLabel->setGeometry(318,70,131,141);
                ui->card1PCLabel->setEnabled(false);
                ui->deathCardPC1Label->setText("Мертвый\nДиссидент");
                return;
            }
            if (isCard == 2)
            {
                lastUseCard = "Диссидент";
                playerPC->count_card--;
                cardMoney(lastUseCard, player, playerPC);
                ui->moneyPCLabel->setText(str.setNum(playerPC->money));
                ui->card2PCLabel->setGeometry(455,70,131,141);
                ui->card2PCLabel->setEnabled(false);
                ui->deathCardPC2Label->setText("Мертвый\nДиссидент");
                return;
            }
        }
        else
        {
            if (isCard == 1)
            {
                playerPC->count_card--;
                ui->card1PCLabel->setGeometry(318,70,131,141);
                ui->card1PCLabel->setEnabled(false);
                ui->deathCardPC1Label->setText("Мертвая карта");
                return;
            }
            if (isCard == 2)
            {
                playerPC->count_card--;
                ui->card2PCLabel->setGeometry(455,70,131,141);
                ui->card2PCLabel->setEnabled(false);
                ui->deathCardPC2Label->setText("Мертвая карта");
                return;
            }
        }
    }
    if (playerPC->count_card == 1)
    {
        if (ui->card1PCLabel->isEnabled())
        {
            playerPC->count_card--;
            ui->card1PCLabel->setGeometry(318,70,131,141);
            ui->card1PCLabel->setEnabled(false);
            ui->deathCardPC1Label->setText("Мертвая карта");
            return;
        }
        else
        {
            playerPC->count_card--;
            ui->card2PCLabel->setGeometry(455,70,131,141);
            ui->card2PCLabel->setEnabled(false);
            ui->deathCardPC2Label->setText("Мертвая карта");
            return;
        }
    }
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
    if (check1 + check2 == 2)
    {
        statusPC = 1;
        return true;
    }
    statusPC = 0;
    return false;
}

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    initPlayer(player);
    initPlayer(playerPC);
    fillCards(&cards[0]);
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::on_changeButton_clicked()
{
    enabledAct();
    ui->statusPCText->setText("");
    fillCards(&cards[0]);

    ui->checkLabel->setStyleSheet("border-style: solid;"
                                  "border-width: 1px;"
                                  "border-color: black;"
                                  "background-color: white;");

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
    if (statusPC == 1)
    {
        ui->statusPCText->setText("");
        typeCardPlayer = ui->card1Button->text();
        QString str;
        if ((lastUseCard == typeCardPlayer) && (typeCardPlayer != "Диссидент"))
        {
            cardMoney(typeCardPlayer, playerPC, player);
            ui->moneyLabel->setText(str.setNum(player->money));
            ui->moneyPCLabel->setText(str.setNum(playerPC->money));
            statusPC = 0;
            enabledAct();
            dropCardPC();
            return;
        }
        else if (lastUseCard == "Диссидент")
        {
            ui->card1Button->setGeometry(10,390,131,141);
            ui->card1Button->setEnabled(false);
            ui->deathCard1Label->setText("Мертвый\nДиссидент");
            player->count_card--;
            if (checkingPC())
            {
                ui->statusPCText->setText("ПРОВЕРЯЮ!");
                if(typeCardPlayer == lastUseCard)
                {
                    ui->deathCard1Label->setText("Мертвая карта");
                    cardMoney(typeCardPlayer, playerPC, player);
                    ui->moneyLabel->setText(str.setNum(player->money));
                    statusPC = 0;
                    enabledAct();
                    dropCardPC();
                    return;
                }
                else
                {
                    ui->deathCard1Label->setText("Мертвая карта");
                    ui->card2Button->setGeometry(150,390,131,141);
                    ui->card2Button->setEnabled(false);
                    ui->deathCard2Label->setText("Мертвая карта");
                    player->count_card--;
                    statusPC = 0;
                    enabledAct();
                    return;
                }
            }
            else
            {
                cardMoney(lastUseCard, playerPC, player);
                ui->moneyLabel->setText(str.setNum(player->money));
                statusPC = 0;
                enabledAct();
                return;
            }
        }
        else
        {
            ui->card1Button->setGeometry(10,390,131,141);
            ui->card1Button->setEnabled(false);
            ui->deathCard1Label->setText("Мертвая карта");
            player->count_card--;
            statusPC = 0;
            enabledAct();
            return;
        }
    }
    else return;
}

void GameWindow::on_card2Button_clicked()
{
    if (statusPC == 1)
    {
        ui->statusPCText->setText("");
        typeCardPlayer = ui->card2Button->text();
        QString str;
        if ((lastUseCard == typeCardPlayer) && (typeCardPlayer != "Диссидент"))
        {
            cardMoney(typeCardPlayer, playerPC, player);
            ui->moneyLabel->setText(str.setNum(player->money));
            ui->moneyPCLabel->setText(str.setNum(playerPC->money));
            statusPC = 0;
            enabledAct();
            dropCardPC();
            return;
        }
        else if (lastUseCard == "Диссидент")
        {
            ui->card2Button->setGeometry(150,390,131,141);
            ui->card2Button->setEnabled(false);
            ui->deathCard2Label->setText("Мертвый\nДиссидент");
            player->count_card--;
            if (checkingPC())
            {
                ui->statusPCText->setText("ПРОВЕРЯЮ!");
                if(typeCardPlayer == lastUseCard)
                {
                    ui->deathCard2Label->setText("Мертвая карта");
                    cardMoney(lastUseCard, playerPC, player);
                    ui->moneyLabel->setText(str.setNum(player->money));
                    statusPC = 0;
                    enabledAct();
                    dropCardPC();
                    return;
                }
                else
                {
                    ui->deathCard2Label->setText("Мертвая карта");
                    ui->card1Button->setGeometry(10,390,131,141);
                    ui->card1Button->setEnabled(false);
                    ui->deathCard1Label->setText("Мертвая карта");
                    player->count_card--;
                    statusPC = 0;
                    enabledAct();
                    return;
                }
            }
            else
            {
                cardMoney(lastUseCard, playerPC, player);
                ui->moneyLabel->setText(str.setNum(player->money));
                statusPC = 0;
                enabledAct();
                return;
            }
        }
        else
        {
            ui->card2Button->setGeometry(150,390,131,141);
            ui->card2Button->setEnabled(false);
            ui->deathCard2Label->setText("Мертвая карта");
            player->count_card--;
            statusPC = 0;
            enabledAct();
            return;
        }
    }
    else return;
}

void GameWindow::on_greenButton_clicked()
{
    QString str;
    typeCardPlayer = "Предприниматель";
    lastUseCard = typeCardPlayer;
    if (checkingPC())
    {
        ui->statusPCText->setText("ПРОВЕРЯЮ!");
        enabledAct();
        return;
    }
    else
    {
        cardMoney(typeCardPlayer, playerPC, player);
        ui->moneyLabel->setText(str.setNum(player->money));
        enabledAct();
        //Ход компьютера
    }
}

void GameWindow::on_reactButton_clicked()
{
    if (statusPC != 1)
    {
        QMessageBox::critical(this, "Предупреждение", "Нельзя ходить картой реакции!");
        return;
    }
    typeCardPlayer = "Диссидент";
    lastUseCard = typeCardPlayer;
    ui->reactButton->setEnabled(false);
    return;
}

/*
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

    if (checkingPC())
    {
        ui->statusPCText->setText("ПРОВЕРЯЮ!");
    }
    else
    {
        cardMoney(typeCardPlayer, player);
        ui->moneyLabel->setText(str.setNum(player->money));
    }
*/

void GameWindow::on_redButton_clicked()
{
    QString str;
    typeCardPlayer = "Бюрократ";
    lastUseCard = typeCardPlayer;
    if (checkingPC())
    {
        ui->statusPCText->setText("ПРОВЕРЯЮ!");
        enabledAct();
        return;
    }
    else
    {
        cardMoney(typeCardPlayer, playerPC, player);
        ui->moneyLabel->setText(str.setNum(player->money));
        ui->moneyPCLabel->setText(str.setNum(playerPC->money));
        enabledAct();
        //Ход компьютера
    }
}

void GameWindow::on_yellowButton_clicked()
{

}

void GameWindow::on_blueButton_clicked()
{

}

void GameWindow::on_money1Button_clicked()
{
    QString str;
    typeCardPlayer = "1 монета";
    lastUseCard = typeCardPlayer;
    player->money++;
    ui->moneyLabel->setText(str.setNum(player->money));
    enabledAct();
    //Ход компьютера
}

void GameWindow::on_restructButton_clicked()
{
    QString str;
    typeCardPlayer = "Перестройка";
    lastUseCard = typeCardPlayer;
    player->money -= 7;
    ui->moneyLabel->setText(str.setNum(player->money));
    enabledAct();
    dropCardPC();
    //Ход компьютера
}

void GameWindow::on_checkButton_clicked()
{

}
