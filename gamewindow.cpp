#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "random.h"

static int cards[15]; //Колода карт
QString lastUseCard;

Player *player = (Player*) malloc(sizeof(Player));
QString typeCardPlayer;
bool statusPresentTV = false;
bool isFirstCardToExchange = false; //Имеется ли карта для замены
int firstCardToExchange; //Индекс карты которая будет менятся

Player *playerPC = (Player*) malloc(sizeof(Player));
QString typeCardPC1;
QString typeCardPC2;
int statusPC = 0; //0 - ожидание, 1 - проверка или сделать ход действием "перестройка", 2 - использование карты
bool isDropCardBlef = false;

void initPlayer(Player *player)
{
    player->money = 2;
    player->count_card = 2;
}

void swapCards(int *from, int *to)
{
    if (from == to) return;
    int tmp = *from;
    *from = *to;
    *to = tmp;
}

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
    ui->greenButton->setEnabled(true);      //
    ui->yellowButton->setEnabled(true);     //
    ui->blueButton->setEnabled(true);       //
    ui->redButton->setEnabled(true);        //
    ui->reactButton->setEnabled(true);      //    Необходимо для одинакового стиля неактивных кнопок
    ui->money1Button->setEnabled(true);     //
    ui->restructButton->setEnabled(true);   //
    ui->checkButton->setEnabled(true);      //
    ui->not_checkButton->setEnabled(true);  //

    if (statusPC == 2)
    {
        ui->greenButton->setEnabled(false);
        ui->yellowButton->setEnabled(false);
        ui->blueButton->setEnabled(false);
        ui->redButton->setEnabled(false);
        ui->reactButton->setEnabled(false);
        ui->money1Button->setEnabled(false);
        ui->restructButton->setEnabled(false);
        ui->checkButton->setEnabled(true);
        ui->not_checkButton->setEnabled(true);
        return;
    }
    if((statusPC == 1) && (player->count_card == 2))
    {
        ui->greenButton->setEnabled(false);
        ui->yellowButton->setEnabled(false);
        ui->blueButton->setEnabled(false);
        ui->redButton->setEnabled(false);
        ui->reactButton->setEnabled(true);
        ui->money1Button->setEnabled(false);
        ui->restructButton->setEnabled(false);
        ui->checkButton->setEnabled(false);
        ui->not_checkButton->setEnabled(false);
        return;
    }
    if((statusPC == 1) && (player->count_card == 1))
    {
        ui->greenButton->setEnabled(false);
        ui->yellowButton->setEnabled(false);
        ui->blueButton->setEnabled(false);
        ui->redButton->setEnabled(false);
        ui->reactButton->setEnabled(false);
        ui->money1Button->setEnabled(false);
        ui->restructButton->setEnabled(false);
        ui->checkButton->setEnabled(false);
        ui->not_checkButton->setEnabled(false);
        return;
    }
    if (statusPresentTV)
    {
        ui->greenButton->setEnabled(false);
        ui->yellowButton->setEnabled(false);
        ui->blueButton->setEnabled(false);
        ui->redButton->setEnabled(false);
        ui->reactButton->setEnabled(false);
        ui->money1Button->setEnabled(false);
        ui->restructButton->setEnabled(false);
        ui->checkButton->setEnabled(false);
        ui->not_checkButton->setEnabled(false);
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
        ui->not_checkButton->setEnabled(false);
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
        ui->not_checkButton->setEnabled(false);
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
        ui->not_checkButton->setEnabled(false);
        return;
    }
    if (player->money < 10)
    {
        ui->greenButton->setEnabled(true);
        ui->yellowButton->setEnabled(true);
        ui->blueButton->setEnabled(true);
        ui->redButton->setEnabled(true);
        ui->reactButton->setEnabled(false);
        ui->money1Button->setEnabled(true);
        ui->restructButton->setEnabled(true);
        ui->checkButton->setEnabled(false);
        ui->not_checkButton->setEnabled(false);
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
        ui->not_checkButton->setEnabled(false);
//        ui->greenButton->setEnabled(false);
//        ui->yellowButton->setEnabled(false);
//        ui->blueButton->setEnabled(false);
//        ui->redButton->setEnabled(false);
//        ui->reactButton->setEnabled(false);
//        ui->money1Button->setEnabled(false);
//        ui->restructButton->setEnabled(true);
//        ui->checkButton->setEnabled(false);
//        ui->not_checkButton->setEnabled(false);
        return;
    }
}

void GameWindow::useDissedentPC(bool isDiss)
{
    QString str;
    if (typeCardPC1 == "Диссидент")
    {
        lastUseCard = typeCardPC1;
        ui->statusPCText->setText("Мертвый Диссидент");
        statusPC = 2;
        enabledAct();
        playerPC->count_card--;
        cardMoney(typeCardPC1, player, playerPC);
        ui->moneyPCLabel->setText(str.setNum(playerPC->money));
        ui->card1PCLabel->setGeometry(318,70,131,141);
        ui->card1PCLabel->setEnabled(false);
        ui->deathCardPC1Label->setText("Мертвая карта");
        return;
    }
    if (typeCardPC2 == "Диссидент")
    {
        lastUseCard = typeCardPC2;
        ui->statusPCText->setText("Мертвый Диссидент");
        statusPC = 2;
        enabledAct();
        playerPC->count_card--;
        cardMoney(typeCardPC2, player, playerPC);
        ui->moneyPCLabel->setText(str.setNum(playerPC->money));
        ui->card2PCLabel->setGeometry(455,70,131,141);
        ui->card2PCLabel->setEnabled(false);
        ui->deathCardPC2Label->setText("Мертвая карта");
        return;
    }
    int isCard = Random(1,2);
    if (isDiss)
    {
        if (isCard == 1)
        {
            lastUseCard = "Диссидент";
            ui->statusPCText->setText("Мертвый Диссидент");
            statusPC = 2;
            enabledAct();
            playerPC->count_card--;
            cardMoney(lastUseCard, player, playerPC);
            ui->moneyPCLabel->setText(str.setNum(playerPC->money));
            ui->card1PCLabel->setGeometry(318,70,131,141);
            ui->card1PCLabel->setEnabled(false);
            ui->deathCardPC1Label->setText("Мертвая карта");
            return;
        }
        if (isCard == 2)
        {
            lastUseCard = "Диссидент";
            ui->statusPCText->setText("Мертвый Диссидент");
            statusPC = 2;
            enabledAct();
            playerPC->count_card--;
            cardMoney(lastUseCard, player, playerPC);
            ui->moneyPCLabel->setText(str.setNum(playerPC->money));
            ui->card2PCLabel->setGeometry(455,70,131,141);
            ui->card2PCLabel->setEnabled(false);
            ui->deathCardPC2Label->setText("Мертвая карта");
            return;
        }
    }
    else
    {
        if (isCard == 1)
        {
            ui->statusPCText->setText("");
            statusPC = 0;
            playerPC->count_card--;
            ui->card1PCLabel->setGeometry(318,70,131,141);
            ui->card1PCLabel->setEnabled(false);
            ui->deathCardPC1Label->setText("Мертвая карта");
            //Переворачивание карты
            return;
        }
        if (isCard == 2)
        {
            ui->statusPCText->setText("");
            statusPC = 0;
            playerPC->count_card--;
            ui->card2PCLabel->setGeometry(455,70,131,141);
            ui->card2PCLabel->setEnabled(false);
            ui->deathCardPC2Label->setText("Мертвая карта");
            //Переворачивание карты
            return;
        }
    }
}

void GameWindow::useKillerPC(bool isKiller)
{
    if (typeCardPC1 == "Киллер")
    {
        lastUseCard = typeCardPC1;
        ui->statusPCText->setText("БЛОКИРУЮ!");
        statusPC = 2;
        enabledAct();
        return;
    }
    if (typeCardPC2 == "Киллер")
    {
        lastUseCard = typeCardPC2;
        ui->statusPCText->setText("БЛОКИРУЮ!");
        statusPC = 2;
        enabledAct();
        return;
    }
    int isCard = Random(1,2);
    if (isKiller)
    {
        if (isCard == 1)
        {
            lastUseCard = typeCardPC1;
            ui->statusPCText->setText("БЛОКИРУЮ!");
            statusPC = 2;
            enabledAct();
            return;
        }
        if (isCard == 2)
        {
            lastUseCard = typeCardPC2;
            ui->statusPCText->setText("БЛОКИРУЮ!");
            statusPC = 2;
            enabledAct();
            return;
        }
    }
    else
    {
        return;
    }
}

void GameWindow::dropCardPC()
{
    DefRandom();
    typeCardPC1 = ui->card1PCLabel->text();
    typeCardPC2 = ui->card2PCLabel->text();
    QString str;
    int isDiss = Random(0,1);
    if (playerPC->count_card == 2)
    {
        if (isDropCardBlef)
        {
            useDissedentPC(isDiss);
            return;
        }
        if ((lastUseCard == "Киллер") && (isDropCardBlef == false))
        {
            int isKiller = Random(0,1);
            useKillerPC(isKiller);
            if (isKiller) return;
            else
            {
                useDissedentPC(isDiss);
                return;
            }
        }
        useDissedentPC(isDiss);
        return;
    }
    if (playerPC->count_card == 1)
    {
        if ((lastUseCard == "Киллер") && (isDropCardBlef == false))
        {
            int isKiller = Random(0,1);
            if (isKiller)
            {
                useKillerPC(isKiller);
                return;
            }
        }
        if (ui->card1PCLabel->isEnabled())
        {
            ui->statusPCText->setText("Вы Победили");
            statusPC = 0;
            playerPC->count_card--;
            ui->card1PCLabel->setGeometry(318,70,131,141);
            ui->card1PCLabel->setEnabled(false);
            ui->deathCardPC1Label->setText("Мертвая карта");
            return;
        }
        else
        {
            ui->statusPCText->setText("Вы Победили");
            statusPC = 0;
            playerPC->count_card--;
            ui->card2PCLabel->setGeometry(455,70,131,141);
            ui->card2PCLabel->setEnabled(false);
            ui->deathCardPC2Label->setText("Мертвая карта");
            return;
        }
    }
}

void GameWindow::cardActionPC(QString typeCard)
{
    return;
}

void GameWindow::computerRun()
{
    DefRandom();
    QString str;
    int isBlef = Random(0,1);
    if (playerPC->money >= 10)
    {
        ui->statusPCText->setText("Перестройка");
        playerPC->money -= 7;
        ui->moneyPCLabel->setText(str.setNum(playerPC->money));
        statusPC = 1;
        enabledAct();
        return;
    }
    if (isBlef)
    {
        //Выбирает карту которой будет ходить
        return;
    }
    else
    {
        ui->statusPCText->setText("Возьму монетку");
        playerPC->money++;
        ui->moneyPCLabel->setText(str.setNum(playerPC->money));
        statusPC = 0;
        enabledAct();
        return;
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

void GameWindow::cardActionPlayer(QString typeCard) //Действия карт в виде сценария
{
    QString str;
    if (typeCard == "Предприниматель")
    {
        cardMoney(typeCard, playerPC, player);
        ui->moneyLabel->setText(str.setNum(player->money));
        return;
    }
    if (typeCard == "Киллер")
    {
        cardMoney(typeCard, playerPC, player);
        ui->moneyLabel->setText(str.setNum(player->money));
        dropCardPC();
        return;
    }
    if (typeCard == "Телеведущий")
    {
        if (statusPC == 2) return;
        else
        {
            cardMoney(typeCard, playerPC, player);
            ui->moneyLabel->setText(str.setNum(player->money));
            statusPresentTV = true;
            ui->moneyPCLabel->setText(str.setNum(playerPC->money));
            ui->card1TVButton->setVisible(true);
            ui->card2TVButton->setVisible(true);
            ui->card3TVButton->setVisible(true);
            ui->returnToDeckButton->setVisible(true);
            ui->card1TVButton->setText(cardFromIntToStr(cards[4]));
            ui->card1TVButton->setStyleSheet(cardColor(cards[4]));
            ui->card2TVButton->setText(cardFromIntToStr(cards[5]));
            ui->card2TVButton->setStyleSheet(cardColor(cards[5]));
            ui->card3TVButton->setText(cardFromIntToStr(cards[6]));
            ui->card3TVButton->setStyleSheet(cardColor(cards[6]));
            return;
        }
    }
    if (typeCard == "Бюрократ")
    {
        cardMoney(typeCard, playerPC, player);
        ui->moneyLabel->setText(str.setNum(player->money));
        ui->moneyPCLabel->setText(str.setNum(playerPC->money));
        return;
    }
    if (typeCard == "Диссидент")
    {
        cardMoney(typeCard, playerPC, player);
        ui->moneyLabel->setText(str.setNum(player->money));
        return;
    }
    return;
}

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    ui->card1TVButton->setVisible(false);
    ui->card2TVButton->setVisible(false);
    ui->card3TVButton->setVisible(false);
    ui->returnToDeckButton->setVisible(false);
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
    ui->checkLabel->setText(s0.setNum(cards[0]) + " " + s1.setNum(cards[1]) + "\n" + s2.setNum(cards[2]) + " " + s3.setNum(cards[3]) + "\n" + s4.setNum(cards[4]) + " " +
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
        QString str;
        if ((lastUseCard == ui->card1Button->text()) && (ui->card1Button->text() != "Диссидент"))
        {
            dropCardPC();
            isDropCardBlef = false;
            cardActionPlayer(typeCardPlayer);
            enabledAct();
            computerRun();
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
                isDropCardBlef = true;
                if(ui->card1Button->text() == lastUseCard)
                {
                    ui->deathCard1Label->setText("Мертвая карта");
                    dropCardPC();
                    isDropCardBlef = false;
                    cardActionPlayer(typeCardPlayer);
                    enabledAct();
                    computerRun();
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
                cardActionPlayer(lastUseCard);
                statusPC = 0;
                enabledAct();
                computerRun();
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
            computerRun();
            return;
        }
    }
    else if (statusPresentTV)
    {
        if (isFirstCardToExchange)
        {
            swapCards(&cards[0], &cards[firstCardToExchange]);
            isFirstCardToExchange = false;
            ui->card1Button->setText(cardFromIntToStr(cards[0]));
            ui->card1Button->setStyleSheet(cardColor(cards[0]));
            switch (firstCardToExchange)
            {
            case 0:
                ui->card1Button->setGeometry(ui->card1Button->x() + 5, ui->card1Button->y() + 5, ui->card1Button->width() - 10, ui->card1Button->height() - 10);
                break;
            case 1:
                ui->card2Button->setText(cardFromIntToStr(cards[1]));
                ui->card2Button->setStyleSheet(cardColor(cards[1]));
                ui->card2Button->setGeometry(ui->card2Button->x() + 5, ui->card2Button->y() + 5, ui->card2Button->width() - 10, ui->card2Button->height() - 10);
                break;
            case 4:
                ui->card1TVButton->setText(cardFromIntToStr(cards[4]));
                ui->card1TVButton->setStyleSheet(cardColor(cards[4]));
                ui->card1TVButton->setGeometry(ui->card1TVButton->x() + 5, ui->card1TVButton->y() + 5, ui->card1TVButton->width() - 10, ui->card1TVButton->height() - 10);
                break;
            case 5:
                ui->card2TVButton->setText(cardFromIntToStr(cards[5]));
                ui->card2TVButton->setStyleSheet(cardColor(cards[5]));
                ui->card2TVButton->setGeometry(ui->card2TVButton->x() + 5, ui->card2TVButton->y() + 5, ui->card2TVButton->width() - 10, ui->card2TVButton->height() - 10);
                break;
            case 6:
                ui->card3TVButton->setText(cardFromIntToStr(cards[6]));
                ui->card3TVButton->setStyleSheet(cardColor(cards[6]));
                ui->card3TVButton->setGeometry(ui->card3TVButton->x() + 5, ui->card3TVButton->y() + 5, ui->card3TVButton->width() - 10, ui->card3TVButton->height() - 10);
                break;
            }
            return;
        }
        else
        {
            ui->card1Button->setGeometry(ui->card1Button->x() - 5, ui->card1Button->y() - 5, ui->card1Button->width() + 10, ui->card1Button->height() + 10);
            firstCardToExchange = 0;
            isFirstCardToExchange = true;
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
        QString str;
        if ((lastUseCard == ui->card2Button->text()) && (ui->card2Button->text() != "Диссидент"))
        {
            dropCardPC();
            isDropCardBlef = false;
            cardActionPlayer(typeCardPlayer);
            enabledAct();
            computerRun();
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
                isDropCardBlef = true;
                if(ui->card2Button->text() == lastUseCard)
                {
                    ui->deathCard2Label->setText("Мертвая карта");
                    dropCardPC();
                    isDropCardBlef = false;
                    cardActionPlayer(typeCardPlayer);
                    enabledAct();
                    computerRun();
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
                cardActionPlayer(typeCardPlayer);
                statusPC = 0;
                enabledAct();
                computerRun();
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
            computerRun();
            return;
        }
    }
    else if (statusPresentTV)
    {
        if (isFirstCardToExchange)
        {
            swapCards(&cards[1], &cards[firstCardToExchange]);
            isFirstCardToExchange = false;
            ui->card2Button->setText(cardFromIntToStr(cards[1]));
            ui->card2Button->setStyleSheet(cardColor(cards[1]));
            switch (firstCardToExchange)
            {
            case 1:
                ui->card2Button->setGeometry(ui->card2Button->x() + 5, ui->card2Button->y() + 5, ui->card2Button->width() - 10, ui->card2Button->height() - 10);
                break;
            case 0:
                ui->card1Button->setText(cardFromIntToStr(cards[0]));
                ui->card1Button->setStyleSheet(cardColor(cards[0]));
                ui->card1Button->setGeometry(ui->card1Button->x() + 5, ui->card1Button->y() + 5, ui->card1Button->width() - 10, ui->card1Button->height() - 10);
                break;
            case 4:
                ui->card1TVButton->setText(cardFromIntToStr(cards[4]));
                ui->card1TVButton->setStyleSheet(cardColor(cards[4]));
                ui->card1TVButton->setGeometry(ui->card1TVButton->x() + 5, ui->card1TVButton->y() + 5, ui->card1TVButton->width() - 10, ui->card1TVButton->height() - 10);
                break;
            case 5:
                ui->card2TVButton->setText(cardFromIntToStr(cards[5]));
                ui->card2TVButton->setStyleSheet(cardColor(cards[5]));
                ui->card2TVButton->setGeometry(ui->card2TVButton->x() + 5, ui->card2TVButton->y() + 5, ui->card2TVButton->width() - 10, ui->card2TVButton->height() - 10);
                break;
            case 6:
                ui->card3TVButton->setText(cardFromIntToStr(cards[6]));
                ui->card3TVButton->setStyleSheet(cardColor(cards[6]));
                ui->card3TVButton->setGeometry(ui->card3TVButton->x() + 5, ui->card3TVButton->y() + 5, ui->card3TVButton->width() - 10, ui->card3TVButton->height() - 10);
                break;
            }
            return;
        }
        else
        {
            ui->card2Button->setGeometry(ui->card2Button->x() - 5, ui->card2Button->y() - 5, ui->card2Button->width() + 10, ui->card2Button->height() + 10);
            firstCardToExchange = 1;
            isFirstCardToExchange = true;
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
        isDropCardBlef = true;
        enabledAct();
        return;
    }
    else
    {
        isFirstCardToExchange = false;
        cardActionPlayer(typeCardPlayer);
        enabledAct();
        computerRun();
    }
}

void GameWindow::on_reactButton_clicked()
{
    typeCardPlayer = "Диссидент";
    lastUseCard = typeCardPlayer;
    ui->reactButton->setEnabled(false);
    statusPresentTV = false;
    return;
}

void GameWindow::on_redButton_clicked()
{
    QString str;
    typeCardPlayer = "Бюрократ";
    lastUseCard = typeCardPlayer;
    if (checkingPC())
    {
        ui->statusPCText->setText("ПРОВЕРЯЮ!");
        isDropCardBlef = true;
        enabledAct();
        return;
    }
    else
    {
        isFirstCardToExchange = false;
        cardActionPlayer(typeCardPlayer);
        enabledAct();
        computerRun();
    }
}

void GameWindow::on_yellowButton_clicked()
{
    QString str;
    typeCardPlayer = "Телеведущий";
    lastUseCard = typeCardPlayer;
    statusPresentTV = true;
    if (checkingPC())
    {
        ui->statusPCText->setText("ПРОВЕРЯЮ!");
        isDropCardBlef = true;
        enabledAct();
        return;
    }
    else
    {
        isFirstCardToExchange = false;
        cardActionPlayer(typeCardPlayer);
        enabledAct();
        computerRun();
    }
}

void GameWindow::on_blueButton_clicked()
{
    QString str;
    typeCardPlayer = "Киллер";
    lastUseCard = typeCardPlayer;
    if (checkingPC())
    {
        ui->statusPCText->setText("ПРОВЕРЯЮ!");
        isDropCardBlef = true;
        enabledAct();
        return;
    }
    else
    {
        isFirstCardToExchange = false;
        cardActionPlayer(typeCardPlayer);
        enabledAct();
        computerRun();
    }
}

void GameWindow::on_money1Button_clicked()
{
    QString str;
    typeCardPlayer = "1 монета";
    lastUseCard = typeCardPlayer;
    player->money++;
    ui->moneyLabel->setText(str.setNum(player->money));
    enabledAct();
    isFirstCardToExchange = false;
    computerRun();
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
    isFirstCardToExchange = false;
    computerRun();
}

void GameWindow::on_checkButton_clicked()
{

}

void GameWindow::on_returnToDeckButton_clicked()
{
    int tv1 = cards[4], tv2 = cards[5], tv3 = cards[6];
    for(int i = 4, j = 7; j < 15; i++, j++)
        swapCards(&cards[i], &cards[j]);
    cards[12] = tv1;
    cards[13] = tv2;
    cards[14] = tv3;
    QString s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14;
    ui->checkLabel->setText(s0.setNum(cards[0]) + " " + s1.setNum(cards[1]) + "\n" + s2.setNum(cards[2]) + " " + s3.setNum(cards[3]) + "\n" + s4.setNum(cards[4]) + " " +
                            s5.setNum(cards[5]) + " " + s6.setNum(cards[6]) + " " + s7.setNum(cards[7]) + " " + s8.setNum(cards[8]) + " " + s9.setNum(cards[9]) + "\n" +
                            s10.setNum(cards[10]) + " " + s11.setNum(cards[11]) + " " + s12.setNum(cards[12]) + " " + s13.setNum(cards[13]) + " " + s14.setNum(cards[14]));
    statusPresentTV = false;
    ui->card1TVButton->setVisible(false);
    ui->card2TVButton->setVisible(false);
    ui->card3TVButton->setVisible(false);
    ui->returnToDeckButton->setVisible(false);
    enabledAct();
    computerRun();
}

void GameWindow::on_not_checkButton_clicked()
{
    statusPC = 0;
    ui->statusPCText->setText("");
    if (typeCardPlayer == "Телеведущий")
        cardActionPlayer(typeCardPlayer);
    enabledAct();
}

void GameWindow::on_card1TVButton_clicked()
{
    if (isFirstCardToExchange)
    {
        swapCards(&cards[4], &cards[firstCardToExchange]);
        isFirstCardToExchange = false;
        ui->card1TVButton->setText(cardFromIntToStr(cards[4]));
        ui->card1TVButton->setStyleSheet(cardColor(cards[4]));
        switch (firstCardToExchange)
        {
        case 4:
            ui->card1TVButton->setGeometry(ui->card1TVButton->x() + 5, ui->card1TVButton->y() + 5, ui->card1TVButton->width() - 10, ui->card1TVButton->height() - 10);
            break;
        case 0:
            ui->card1Button->setText(cardFromIntToStr(cards[0]));
            ui->card1Button->setStyleSheet(cardColor(cards[0]));
            ui->card1Button->setGeometry(ui->card1Button->x() + 5, ui->card1Button->y() + 5, ui->card1Button->width() - 10, ui->card1Button->height() - 10);
            break;
        case 1:
            ui->card2Button->setText(cardFromIntToStr(cards[1]));
            ui->card2Button->setStyleSheet(cardColor(cards[1]));
            ui->card2Button->setGeometry(ui->card2Button->x() + 5, ui->card2Button->y() + 5, ui->card2Button->width() - 10, ui->card2Button->height() - 10);
            break;
        case 5:
            ui->card2TVButton->setText(cardFromIntToStr(cards[5]));
            ui->card2TVButton->setStyleSheet(cardColor(cards[5]));
            ui->card2TVButton->setGeometry(ui->card2TVButton->x() + 5, ui->card2TVButton->y() + 5, ui->card2TVButton->width() - 10, ui->card2TVButton->height() - 10);
            break;
        case 6:
            ui->card3TVButton->setText(cardFromIntToStr(cards[6]));
            ui->card3TVButton->setStyleSheet(cardColor(cards[6]));
            ui->card3TVButton->setGeometry(ui->card3TVButton->x() + 5, ui->card3TVButton->y() + 5, ui->card3TVButton->width() - 10, ui->card3TVButton->height() - 10);
            break;
        }
        return;
    }
    else
    {
        ui->card1TVButton->setGeometry(ui->card1TVButton->x() - 5, ui->card1TVButton->y() - 5, ui->card1TVButton->width() + 10, ui->card1TVButton->height() + 10);
        firstCardToExchange = 4;
        isFirstCardToExchange = true;
        return;
    }
}

void GameWindow::on_card2TVButton_clicked()
{
    if (isFirstCardToExchange)
    {
        swapCards(&cards[5], &cards[firstCardToExchange]);
        isFirstCardToExchange = false;
        ui->card2TVButton->setText(cardFromIntToStr(cards[5]));
        ui->card2TVButton->setStyleSheet(cardColor(cards[5]));
        switch (firstCardToExchange)
        {
        case 5:
            ui->card2TVButton->setGeometry(ui->card2TVButton->x() + 5, ui->card2TVButton->y() + 5, ui->card2TVButton->width() - 10, ui->card2TVButton->height() - 10);
            break;
        case 0:
            ui->card1Button->setText(cardFromIntToStr(cards[0]));
            ui->card1Button->setStyleSheet(cardColor(cards[0]));
            ui->card1Button->setGeometry(ui->card1Button->x() + 5, ui->card1Button->y() + 5, ui->card1Button->width() - 10, ui->card1Button->height() - 10);
            break;
        case 1:
            ui->card2Button->setText(cardFromIntToStr(cards[1]));
            ui->card2Button->setStyleSheet(cardColor(cards[1]));
            ui->card2Button->setGeometry(ui->card2Button->x() + 5, ui->card2Button->y() + 5, ui->card2Button->width() - 10, ui->card2Button->height() - 10);
            break;
        case 4:
            ui->card1TVButton->setText(cardFromIntToStr(cards[4]));
            ui->card1TVButton->setStyleSheet(cardColor(cards[4]));
            ui->card1TVButton->setGeometry(ui->card1TVButton->x() + 5, ui->card1TVButton->y() + 5, ui->card1TVButton->width() - 10, ui->card1TVButton->height() - 10);
            break;
        case 6:
            ui->card3TVButton->setText(cardFromIntToStr(cards[6]));
            ui->card3TVButton->setStyleSheet(cardColor(cards[6]));
            ui->card3TVButton->setGeometry(ui->card3TVButton->x() + 5, ui->card3TVButton->y() + 5, ui->card3TVButton->width() - 10, ui->card3TVButton->height() - 10);
            break;
        }
        return;
    }
    else
    {
        ui->card2TVButton->setGeometry(ui->card2TVButton->x() - 5, ui->card2TVButton->y() - 5, ui->card2TVButton->width() + 10, ui->card2TVButton->height() + 10);
        firstCardToExchange = 5;
        isFirstCardToExchange = true;
        return;
    }
}

void GameWindow::on_card3TVButton_clicked()
{
    if (isFirstCardToExchange)
    {
        swapCards(&cards[6], &cards[firstCardToExchange]);
        isFirstCardToExchange = false;
        ui->card3TVButton->setText(cardFromIntToStr(cards[6]));
        ui->card3TVButton->setStyleSheet(cardColor(cards[6]));
        switch (firstCardToExchange)
        {
        case 6:
            ui->card3TVButton->setGeometry(ui->card3TVButton->x() + 5, ui->card3TVButton->y() + 5, ui->card3TVButton->width() - 10, ui->card3TVButton->height() - 10);
            break;
        case 0:
            ui->card1Button->setText(cardFromIntToStr(cards[0]));
            ui->card1Button->setStyleSheet(cardColor(cards[0]));
            ui->card1Button->setGeometry(ui->card1Button->x() + 5, ui->card1Button->y() + 5, ui->card1Button->width() - 10, ui->card1Button->height() - 10);
            break;
        case 1:
            ui->card2Button->setText(cardFromIntToStr(cards[1]));
            ui->card2Button->setStyleSheet(cardColor(cards[1]));
            ui->card2Button->setGeometry(ui->card2Button->x() + 5, ui->card2Button->y() + 5, ui->card2Button->width() - 10, ui->card2Button->height() - 10);
            break;
        case 4:
            ui->card1TVButton->setText(cardFromIntToStr(cards[4]));
            ui->card1TVButton->setStyleSheet(cardColor(cards[4]));
            ui->card1TVButton->setGeometry(ui->card1TVButton->x() + 5, ui->card1TVButton->y() + 5, ui->card1TVButton->width() - 10, ui->card1TVButton->height() - 10);
            break;
        case 5:
            ui->card2TVButton->setText(cardFromIntToStr(cards[5]));
            ui->card2TVButton->setStyleSheet(cardColor(cards[5]));
            ui->card2TVButton->setGeometry(ui->card2TVButton->x() + 5, ui->card2TVButton->y() + 5, ui->card2TVButton->width() - 10, ui->card2TVButton->height() - 10);
            break;
        }
        return;
    }
    else
    {
        ui->card3TVButton->setGeometry(ui->card3TVButton->x() - 5, ui->card3TVButton->y() - 5, ui->card3TVButton->width() + 10, ui->card3TVButton->height() + 10);
        firstCardToExchange = 6;
        isFirstCardToExchange = true;
        return;
    }
}
