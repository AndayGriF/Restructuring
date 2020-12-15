#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "random.h"

static int cards[15]; //Колода карт
static QString lastUseAct;

static Player *player = (Player*) malloc(sizeof(Player));
static QString lastActPlayer;
static bool statusPresentTV = false;
static bool isFirstCardToExchange = false; //Имеется ли карта для замены
static int firstCardToExchange; //Индекс карты которая будет менятся

static Player *playerPC = (Player*) malloc(sizeof(Player));
static QString lastActPC;
static QString typeCardPC1;
static QString typeCardPC2;
static int statusPC = 0;    //  0 - ожидание,
                            //  1 - проверка, сделать ход действием "перестройка",
                            //  2 - использование карты,
                            //  3 - использование карты киллера
                            //  41, 42 - проверка мертвого диссидента 1 и 2 карты
static bool isPCRUN;

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
    check1 = static_cast<int>(Random(0,1));
    check2 = static_cast<int>(Random(0,1));
    if (check1 + check2 == 2)
    {
        statusPC = 1;
        return true;
    }
    statusPC = 0;
    return false;
}

bool isCanUseCardPC (int typeCard)
{
    if (typeCard == 0) return true;
    if ((typeCard == 1) && (playerPC->money >= 4)) return true;
    if ((typeCard == 2) && (playerPC->money >= 1)) return true;
    if (typeCard == 3) return true;
    return false;
}

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

    int i = 0;
    do
    {
       num = static_cast<int>(Random(0, 4));
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

void GameWindow::visibleShirtCard() //Показать рубашку карты
{
    if (ui->card1PCLabel->isEnabled()) ui->shirtCard1PCButton->setVisible(true);
    if (ui->card2PCLabel->isEnabled()) ui->shirtCard2PCButton->setVisible(true);
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

    if ((player->count_card < 1) || (playerPC->count_card < 1))
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
        if (playerPC->count_card < 1)
        {
            ui->statusPCText->setText("Вы Победили");
            return;
        }
        if (player->count_card < 1)
        {
            ui->statusPCText->setText("Вы Проиграли");
            return;
        }
    }

    if ((statusPC == 41) || (statusPC == 42))
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
        ui->card1Button->setEnabled(true);
        ui->card2Button->setEnabled(true);
        return;
    }

    if (statusPC == 3)
    {
        ui->greenButton->setEnabled(false);
        ui->yellowButton->setEnabled(false);
        ui->blueButton->setEnabled(true);
        ui->redButton->setEnabled(false);
        ui->reactButton->setEnabled(true);
        ui->money1Button->setEnabled(false);
        ui->restructButton->setEnabled(false);
        ui->checkButton->setEnabled(true);
        ui->not_checkButton->setEnabled(true);
        return;
    }
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

bool GameWindow::useEndPredPC()
{
    QString str;
    lastUseAct = "Предприниматель";
    lastActPC = "Предприниматель";
    cardMoney(lastActPC, player, playerPC);
    ui->moneyPCLabel->setText(str.setNum(playerPC->money));
    statusPC = 0;
    enabledAct();
    return true;
}

bool GameWindow::useKillerPC()
{
    if (lastActPlayer == "Киллер")
    {
        if ((typeCardPC1 == "Киллер") && (ui->card1PCLabel->isEnabled() == true))
        {
            lastUseAct = typeCardPC1;
            lastActPC = "Блокирую";
            statusPC = 2;
            enabledAct();
            return true;
        }
        if ((typeCardPC2 == "Киллер") && (ui->card2PCLabel->isEnabled() == true))
        {
            lastUseAct = typeCardPC2;
            lastActPC = "Блокирую";
            statusPC = 2;
            enabledAct();
            return true;
        }
        int isBlef = static_cast<int>(Random(0,2));
        if (isBlef)
        {
            lastUseAct = "Киллер";
            lastActPC = "Блокирую";
            statusPC = 2;
            enabledAct();
            return true;
        }
        return false;
    }
    QString str;
    lastUseAct = "Киллер";
    lastActPC = "Киллер";
    cardMoney(lastActPC, player, playerPC);
    ui->moneyPCLabel->setText(str.setNum(playerPC->money));
    statusPC = 3;
    enabledAct();
    return true;
}

bool GameWindow::usePresentTVPC()
{
    QString str;
    lastUseAct = "Телеведущий";
    lastActPC = "Телеведущий";
    cardMoney(lastActPC, player, playerPC);
    ui->moneyPCLabel->setText(str.setNum(playerPC->money));
    unsigned int isChange = Random(0,1);
    if (isChange)
    {
        unsigned int isCardTV;
        if (playerPC->count_card == 2)
        {
            isCardTV = Random(4,6);
            swapCards(&cards[2], &cards[isCardTV]);
            ui->card1PCLabel->setText(cardFromIntToStr(cards[2]));
            isCardTV = Random(4,6);
            swapCards(&cards[3], &cards[isCardTV]);
            ui->card2PCLabel->setText(cardFromIntToStr(cards[3]));
        }
        if (playerPC->count_card == 1)
        {
            if (ui->card1PCLabel->isEnabled())
            {
                isCardTV = Random(4,6);
                swapCards(&cards[2], &cards[isCardTV]);
                ui->card1PCLabel->setText(cardFromIntToStr(cards[2]));
            }
            if (ui->card2PCLabel->isEnabled())
            {
                isCardTV = Random(4,6);
                swapCards(&cards[3], &cards[isCardTV]);
                ui->card2PCLabel->setText(cardFromIntToStr(cards[3]));
            }
        }
    }
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
    statusPC = 0;
    enabledAct();
    return true;
}

bool GameWindow::useBurocratPC()
{
    QString str;
    lastUseAct = "Бюрократ";
    lastActPC = "Бюрократ";
    cardMoney(lastActPC, player, playerPC);
    ui->moneyPCLabel->setText(str.setNum(playerPC->money));
    ui->moneyLabel->setText(str.setNum(player->money));
    statusPC = 0;
    enabledAct();
    return true;
}

bool GameWindow::useDissedentPC()
{
    QString str;
    if (playerPC->count_card < 2) return false;
    if ((typeCardPC1 == "Диссидент") && (ui->card1PCLabel->isEnabled() == true))
    {
        lastUseAct = typeCardPC1;
        lastActPC = "Диссидент";
        statusPC = 2;
        enabledAct();
        playerPC->count_card--;
        ui->card1PCLabel->setGeometry(ui->card1PCLabel->x(), ui->card1PCLabel->y() + 40, ui->card1PCLabel->width(), ui->card1PCLabel->height());
        ui->card1PCLabel->setEnabled(false);
        ui->deathCardPC1Label->setText("Мертвая карта");
        ui->shirtCard1PCButton->setGeometry(ui->card1PCLabel->x(), ui->card1PCLabel->y(), ui->card1PCLabel->width(), ui->card1PCLabel->height());
        return true;
    }
    if ((typeCardPC2 == "Диссидент") && (ui->card2PCLabel->isEnabled() == true))
    {
        lastUseAct = typeCardPC2;
        lastActPC = "Диссидент";
        statusPC = 2;
        enabledAct();
        playerPC->count_card--;
        ui->card2PCLabel->setGeometry(ui->card2PCLabel->x(), ui->card2PCLabel->y() + 40, ui->card2PCLabel->width(), ui->card2PCLabel->height());
        ui->card2PCLabel->setEnabled(false);
        ui->deathCardPC2Label->setText("Мертвая карта");
        ui->shirtCard2PCButton->setGeometry(ui->card2PCLabel->x(), ui->card2PCLabel->y(), ui->card2PCLabel->width(), ui->card2PCLabel->height());
        return true;
    }
    int isCard = static_cast<int>(Random(1,2));
    int isDiss = static_cast<int>(Random(0,1));
    if (isDiss && (playerPC->count_card == 2))
    {
        if (isCard == 1)
        {
            lastUseAct = "Диссидент";
            lastActPC = "Диссидент";
            statusPC = 2;
            enabledAct();
            playerPC->count_card--;
            ui->card1PCLabel->setGeometry(ui->card1PCLabel->x(), ui->card1PCLabel->y() + 40, ui->card1PCLabel->width(), ui->card1PCLabel->height());
            ui->card1PCLabel->setEnabled(false);
            ui->deathCardPC1Label->setText("Мертвая карта");
            ui->shirtCard1PCButton->setGeometry(ui->card1PCLabel->x(), ui->card1PCLabel->y(), ui->card1PCLabel->width(), ui->card1PCLabel->height());
            return true;
        }
        if (isCard == 2)
        {
            lastUseAct = "Диссидент";
            lastActPC = "Диссидент";
            statusPC = 2;
            enabledAct();
            playerPC->count_card--;
            ui->card2PCLabel->setGeometry(ui->card2PCLabel->x(), ui->card2PCLabel->y() + 40, ui->card2PCLabel->width(), ui->card2PCLabel->height());
            ui->card2PCLabel->setEnabled(false);
            ui->deathCardPC2Label->setText("Мертвая карта");
            ui->shirtCard2PCButton->setGeometry(ui->card2PCLabel->x(), ui->card2PCLabel->y(), ui->card2PCLabel->width(), ui->card2PCLabel->height());
            return true;
        }
    }
    return false;
}

void GameWindow::swapCardPC()
{
    if (playerPC->count_card != 2) return;
    unsigned int isChange = Random(0,1);
    if (isChange)
    {
        swapCards(&cards[2], &cards[3]);
        ui->card1PCLabel->setText(cardFromIntToStr(cards[2]));
        ui->card2PCLabel->setText(cardFromIntToStr(cards[3]));
    }
}

void GameWindow::dropCardPC()
{
    QString str;
    if ((lastActPlayer == "Киллер") && (useKillerPC() == true))
    {
        ui->statusPCText->setText("Блокирую");
        return;
    }
    if (useDissedentPC())
    {ui->shirtCard1PCButton->setGeometry(ui->card1PCLabel->x(), ui->card1PCLabel->y(), ui->card1PCLabel->width(), ui->card1PCLabel->height());
        ui->statusPCText->setText("Мертвый Диссидент");
        statusPC = 2;
        enabledAct();
        return;
    }
    if (playerPC->count_card == 2)
    {
        int isCard = static_cast<int>(Random(1,2));
        if (isCard == 1)
        {
            ui->statusPCText->setText("");
            statusPC = 0;
            playerPC->count_card--;
            ui->card1PCLabel->setGeometry(ui->card1PCLabel->x(), ui->card1PCLabel->y() + 40, ui->card1PCLabel->width(), ui->card1PCLabel->height());
            ui->card1PCLabel->setEnabled(false);
            ui->deathCardPC1Label->setText("Мертвая карта");
            ui->shirtCard1PCButton->setVisible(false);
            return;
        }
        if (isCard == 2)
        {
            ui->statusPCText->setText("");
            statusPC = 0;
            playerPC->count_card--;
            ui->card2PCLabel->setGeometry(ui->card2PCLabel->x(), ui->card2PCLabel->y() + 40, ui->card2PCLabel->width(), ui->card2PCLabel->height());
            ui->card2PCLabel->setEnabled(false);
            ui->deathCardPC2Label->setText("Мертвая карта");
            ui->shirtCard2PCButton->setVisible(false);
            return;
        }
    }
    if (playerPC->count_card == 1)
    {
        if (ui->card1PCLabel->isEnabled())
        {
            ui->statusPCText->setText("");
            statusPC = 0;
            playerPC->count_card--;
            ui->card1PCLabel->setGeometry(ui->card1PCLabel->x(), ui->card1PCLabel->y() + 40, ui->card1PCLabel->width(), ui->card1PCLabel->height());
            ui->card1PCLabel->setEnabled(false);
            ui->deathCardPC1Label->setText("Мертвая карта");
            ui->shirtCard1PCButton->setVisible(false);
            return;
        }
        if (ui->card2PCLabel->isEnabled())
        {
            ui->statusPCText->setText("");
            statusPC = 0;
            playerPC->count_card--;
            ui->card2PCLabel->setGeometry(ui->card2PCLabel->x(), ui->card2PCLabel->y() + 40, ui->card2PCLabel->width(), ui->card2PCLabel->height());
            ui->card2PCLabel->setEnabled(false);
            ui->deathCardPC2Label->setText("Мертвая карта");
            ui->shirtCard2PCButton->setVisible(false);
            return;
        }
    }
}

void GameWindow::computerCardRun(int isCard)
{
    if (isCard == 0)
    {
        ui->statusPCText->setText(cardFromIntToStr(isCard));
        lastActPC = "Предприниматель";
        statusPC = 2;
        enabledAct();
        return;
    }
    if (isCard == 1)
    {
        ui->statusPCText->setText(cardFromIntToStr(isCard));
        lastActPC = "Киллер";
        statusPC = 3;
        enabledAct();
        return;
    }
    if (isCard == 2)
    {
        ui->statusPCText->setText(cardFromIntToStr(isCard));
        lastActPC = "Телеведущий";
        statusPC = 2;
        enabledAct();
        return;
    }
    if (isCard == 3)
    {
        ui->statusPCText->setText(cardFromIntToStr(isCard));
        lastActPC = "Бюрократ";
        statusPC = 2;
        enabledAct();
        return;
    }
}

void GameWindow::computerRun(bool isRun) //Ход компьютера
{
    if (playerPC->count_card < 1) return;
    if (isRun == false)
    {
        statusPC = 0;
        enabledAct();
        return;
    }
    isRun = false;
    typeCardPC1 = ui->card1PCLabel->text();
    typeCardPC2 = ui->card2PCLabel->text();
    QString str;
    if (playerPC->money >= 7)
    {
        ui->statusPCText->setText("Перестройка");
        playerPC->money -= 7;
        ui->moneyPCLabel->setText(str.setNum(playerPC->money));
        statusPC = 1;
        enabledAct();
        return;
    }
    int isCard;
    unsigned int isBlef = Random(0,1);
    if (isBlef)
    {
        do
        {
            isCard = static_cast<int>(Random(0,3));
        } while (isCanUseCardPC(isCard) == false);
        computerCardRun(isCard);
        return;
    }
    else
    {
        isCard = static_cast<int>(Random(1,2));
        if (isCard == 1)
        {
            if (ui->card1PCLabel->isEnabled() && isCanUseCardPC(cards[2]))
            {
                computerCardRun(cards[2]);
                return;
            }
            if (ui->card2PCLabel->isEnabled() && isCanUseCardPC(cards[3]))
            {
                computerCardRun(cards[3]);
                return;
            }
        }
        if (isCard == 2)
        {
            if (ui->card2PCLabel->isEnabled() && isCanUseCardPC(cards[3]))
            {
                computerCardRun(cards[3]);
                return;
            }
            if (ui->card1PCLabel->isEnabled() && isCanUseCardPC(cards[2]))
            {
                computerCardRun(cards[2]);
                return;
            }
        }
        ui->statusPCText->setText("Возьму монетку");
        playerPC->money++;
        ui->moneyPCLabel->setText(str.setNum(playerPC->money));
        statusPC = 0;
        enabledAct();
        return;
    }
}

int GameWindow::checkCardPC(QString typeCard)
{
    QString str;
    if ((ui->card1PCLabel->text() == typeCard) && (ui->card1PCLabel->isEnabled() == true)) return 1;
    if ((ui->card2PCLabel->text() == typeCard) && (ui->card2PCLabel->isEnabled() == true)) return 2;
    return 0;
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
            statusPresentTV = true;
            cardMoney(typeCard, playerPC, player);
            ui->moneyLabel->setText(str.setNum(player->money));
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
    DefRandom();
    ui->card1TVButton->setVisible(false);
    ui->card2TVButton->setVisible(false);
    ui->card3TVButton->setVisible(false);
    ui->returnToDeckButton->setVisible(false);
    initPlayer(player);
    initPlayer(playerPC);
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
    ui->shirtCard1PCButton->setStyleSheet("border-style: solid;"
                                          "border-width: 3px;"
                                          "border-color: black;"
                                          "border-radius: 10px;"
                                          "background-color: red");
    ui->shirtCard2PCButton->setStyleSheet("border-style: solid;"
                                          "border-width: 3px;"
                                          "border-color: black;"
                                          "border-radius: 10px;"
                                          "background-color: red");
    QString s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, color;
    ui->checkLabel->setText(s0.setNum(cards[0]) + " " + s1.setNum(cards[1]) + "\n" + s2.setNum(cards[2]) + " " + s3.setNum(cards[3]) + "\n" + s4.setNum(cards[4]) + " " +
                            s5.setNum(cards[5]) + " " + s6.setNum(cards[6]) + " " + s7.setNum(cards[7]) + " " + s8.setNum(cards[8]) + " " + s9.setNum(cards[9]) + "\n" +
                            s10.setNum(cards[10]) + " " + s11.setNum(cards[11]) + " " + s12.setNum(cards[12]) + " " + s13.setNum(cards[13]) + " " + s14.setNum(cards[14]));
    ui->card1Button->setText(cardFromIntToStr(cards[0]));
    ui->card1Button->setStyleSheet(cardColor(cards[0]));
    ui->card2Button->setText(cardFromIntToStr(cards[1]));
    ui->card2Button->setStyleSheet(cardColor(cards[1]));
    ui->card1PCLabel->setText(cardFromIntToStr(cards[2]));
    ui->card2PCLabel->setText(cardFromIntToStr(cards[3]));
    enabledAct();
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::dropCardPlayer(int num_card)
{
    player->count_card--;
    if (num_card == 1)
    {
        ui->card1Button->setGeometry(ui->card1Button->x(), ui->card1Button->y() - 40, ui->card1Button->width(), ui->card1Button->height());
        ui->card1Button->setEnabled(false);
        ui->deathCard1Label->setText("Мертвая карта");
        return;
    }
    if (num_card == 2)
    {
        ui->card2Button->setGeometry(ui->card2Button->x(), ui->card2Button->y() - 40, ui->card2Button->width(), ui->card2Button->height());
        ui->card2Button->setEnabled(false);
        ui->deathCard2Label->setText("Мертвая карта");
        return;
    }
}

void GameWindow::on_changeButton_clicked()
{
    QString s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, str;
    fillCards(&cards[0]);
    initPlayer(player);
    initPlayer(playerPC);
    ui->statusPCText->setText("");
    ui->card1Button->setEnabled(true);
    ui->card2Button->setEnabled(true);
    ui->card1PCLabel->setEnabled(true);
    ui->card2PCLabel->setEnabled(true);
    ui->card1Button->setGeometry(10,420,141,151);
    ui->card2Button->setGeometry(160,420,141,151);
    ui->card1PCLabel->setGeometry(315,10,141,151);
    ui->card2PCLabel->setGeometry(465,10,141,151);
    ui->deathCard1Label->setText("");
    ui->deathCard2Label->setText("");
    ui->deathCardPC1Label->setText("");
    ui->deathCardPC2Label->setText("");
    ui->moneyLabel->setText(str.setNum(player->money));
    ui->moneyPCLabel->setText(str.setNum(playerPC->money));
    ui->card1Button->setText(cardFromIntToStr(cards[0]));
    ui->card1Button->setStyleSheet(cardColor(cards[0]));
    ui->card2Button->setText(cardFromIntToStr(cards[1]));
    ui->card2Button->setStyleSheet(cardColor(cards[1]));
    ui->card1PCLabel->setText(cardFromIntToStr(cards[2]));
    ui->card2PCLabel->setText(cardFromIntToStr(cards[3]));
    ui->checkLabel->setText(s0.setNum(cards[0]) + " " + s1.setNum(cards[1]) + "\n" +
                            s2.setNum(cards[2]) + " " + s3.setNum(cards[3]) + "\n" +
                            s4.setNum(cards[4]) + " " + s5.setNum(cards[5]) + " " + s6.setNum(cards[6]) + " " + s7.setNum(cards[7]) + " " + s8.setNum(cards[8]) + " " + s9.setNum(cards[9]) + "\n" +
                            s10.setNum(cards[10]) + " " + s11.setNum(cards[11]) + " " + s12.setNum(cards[12]) + " " + s13.setNum(cards[13]) + " " + s14.setNum(cards[14]));

    enabledAct();
}

void GameWindow::on_card1Button_clicked()
{
    visibleShirtCard();
    if (statusPC == 41)
    {
        ui->statusPCText->setText("");
        statusPC = 0;
        if(ui->card1Button->text() == lastActPlayer)
        {
            ui->card1Button->setEnabled(false);
            ui->deathCard1Label->setText("Мертвая карта");
            dropCardPC();
            cardActionPlayer(lastActPlayer);
            enabledAct();
            computerRun(isPCRUN);
            return;
        }
        else
        {
            ui->card1Button->setEnabled(false);
            ui->deathCard1Label->setText("Мертвая карта");
            dropCardPlayer(2);
            enabledAct();
            return;
        }
    }
    if (statusPC == 42)
    {
        ui->deathCard2Label->setText("Мертвая карта");
        dropCardPlayer(1);
        enabledAct();
        return;
    }
    if ((statusPC == 1) || (statusPC == 3))
    {
        ui->statusPCText->setText("");
        QString str;
        if (lastActPlayer == "Диссидент")
        {
            dropCardPlayer(1);
            ui->deathCard1Label->setText("Мертвый\nДиссидент");
            if (checkingPC())
            {
                ui->statusPCText->setText("Проверяю");
                statusPC = 41;
                enabledAct();
                return;
            }
            else
            {
                if (lastActPC == "Киллер")
                {
                    statusPC = 0;
                    cardActionPlayer(lastActPlayer);
                    enabledAct();
                    return;
                }
                statusPC = 0;
                cardActionPlayer(lastActPlayer);
                enabledAct();
                computerRun(isPCRUN);
                return;
            }
        }
        if (((lastActPlayer == ui->card1Button->text()) && (ui->card1Button->text() != "Диссидент")) || ((lastActPlayer == "Блокирую") && (ui->card1Button->text() == "Киллер")))
        {
            if (lastActPlayer == "Киллер")
            {
                if (playerPC->count_card == 2)
                {
                    ui->statusPCText->setText("");
                    playerPC->count_card--;
                    ui->card1PCLabel->setGeometry(ui->card1PCLabel->x(), ui->card1PCLabel->y() + 40, ui->card1PCLabel->width(), ui->card1PCLabel->height());
                    ui->card1PCLabel->setEnabled(false);
                    ui->deathCardPC1Label->setText("Мертвая карта");
                    //Переворачивание карты

                    ui->statusPCText->setText("");
                    playerPC->count_card--;
                    ui->card2PCLabel->setGeometry(ui->card2PCLabel->x(), ui->card2PCLabel->y() + 40, ui->card2PCLabel->width(), ui->card2PCLabel->height());
                    ui->card2PCLabel->setEnabled(false);
                    ui->deathCardPC2Label->setText("Мертвая карта");
                    //Переворачивание карты
                    enabledAct();
                    return;
                }
                else
                {
                    if (ui->card1PCLabel->isEnabled())
                    {
                        ui->statusPCText->setText("");
                        playerPC->count_card--;
                        ui->card1PCLabel->setGeometry(ui->card1PCLabel->x(), ui->card1PCLabel->y() + 40, ui->card1PCLabel->width(), ui->card1PCLabel->height());
                        ui->card1PCLabel->setEnabled(false);
                        ui->deathCardPC1Label->setText("Мертвая карта");
                        //Переворачивание карты
                        enabledAct();
                        return;
                    }
                    if (ui->card2PCLabel->isEnabled())
                    {
                        ui->statusPCText->setText("");
                        playerPC->count_card--;
                        ui->card2PCLabel->setGeometry(ui->card2PCLabel->x(), ui->card2PCLabel->y() + 40, ui->card2PCLabel->width(), ui->card2PCLabel->height());
                        ui->card2PCLabel->setEnabled(false);
                        ui->deathCardPC2Label->setText("Мертвая карта");
                        //Переворачивание карты
                        enabledAct();
                        return;
                    }
                }
            }
            dropCardPC();
            if (statusPC == 3)
            {
                statusPC = 0;
                enabledAct();
                return;
            }
            else
            {
                statusPC = 0;
                cardActionPlayer(lastActPlayer);
                enabledAct();
                computerRun(isPCRUN);
                return;
            }
        }
        else
        {
            dropCardPlayer(1);
            statusPC = 0;
            enabledAct();
            computerRun(isPCRUN);
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
    visibleShirtCard();
    if (statusPC == 42)
    {
        ui->statusPCText->setText("");
        statusPC = 0;
        if(ui->card2Button->text() == lastActPlayer)
        {
            ui->card2Button->setEnabled(false);
            ui->deathCard2Label->setText("Мертвая карта");
            dropCardPC();
            cardActionPlayer(lastActPlayer);
            enabledAct();
            computerRun(isPCRUN);
            return;
        }
        else
        {
            ui->card2Button->setEnabled(false);
            ui->deathCard2Label->setText("Мертвая карта");
            dropCardPlayer(1);
            enabledAct();
            return;
        }
    }
    if (statusPC == 41)
    {
        ui->deathCard1Label->setText("Мертвая карта");
        dropCardPlayer(2);
        enabledAct();
        return;
    }
    if ((statusPC == 1) || (statusPC == 3))
    {
        ui->statusPCText->setText("");
        QString str;
        if (lastActPlayer == "Диссидент")
        {
            dropCardPlayer(2);
            ui->deathCard2Label->setText("Мертвый\nДиссидент");
            if (checkingPC())
            {
                ui->statusPCText->setText("Проверяю");
                statusPC = 42;
                enabledAct();
                return;
            }
            else
            {
                if (lastActPC == "Киллер")
                {
                    statusPC = 0;
                    cardActionPlayer(lastActPlayer);
                    enabledAct();
                    return;
                }
                statusPC = 0;
                cardActionPlayer(lastActPlayer);
                enabledAct();
                computerRun(isPCRUN);
                return;
            }
        }
        if (((lastActPlayer == ui->card2Button->text()) && (ui->card2Button->text() != "Диссидент")) || ((lastActPlayer == "Блокирую") && (ui->card2Button->text() == "Киллер")))
        {
            if (lastActPlayer == "Киллер")
            {
                if (playerPC->count_card == 2)
                {
                    ui->statusPCText->setText("");
                    playerPC->count_card--;
                    ui->card1PCLabel->setGeometry(ui->card1PCLabel->x(), ui->card1PCLabel->y() + 40, ui->card1PCLabel->width(), ui->card1PCLabel->height());
                    ui->card1PCLabel->setEnabled(false);
                    ui->deathCardPC1Label->setText("Мертвая карта");
                    //Переворачивание карты

                    ui->statusPCText->setText("");
                    playerPC->count_card--;
                    ui->card2PCLabel->setGeometry(ui->card2PCLabel->x(), ui->card2PCLabel->y() + 40, ui->card2PCLabel->width(), ui->card2PCLabel->height());
                    ui->card2PCLabel->setEnabled(false);
                    ui->deathCardPC2Label->setText("Мертвая карта");
                    //Переворачивание карты
                    enabledAct();
                    return;
                }
                else
                {
                    if (ui->card1PCLabel->isEnabled())
                    {
                        ui->statusPCText->setText("");
                        playerPC->count_card--;
                        ui->card1PCLabel->setGeometry(ui->card1PCLabel->x(), ui->card1PCLabel->y() + 40, ui->card1PCLabel->width(), ui->card1PCLabel->height());
                        ui->card1PCLabel->setEnabled(false);
                        ui->deathCardPC1Label->setText("Мертвая карта");
                        //Переворачивание карты
                        enabledAct();
                        return;
                    }
                    if (ui->card2PCLabel->isEnabled())
                    {
                        ui->statusPCText->setText("");
                        playerPC->count_card--;
                        ui->card2PCLabel->setGeometry(ui->card2PCLabel->x(), ui->card2PCLabel->y() + 40, ui->card2PCLabel->width(), ui->card2PCLabel->height());
                        ui->card2PCLabel->setEnabled(false);
                        ui->deathCardPC2Label->setText("Мертвая карта");
                        //Переворачивание карты
                        enabledAct();
                        return;
                    }
                }
            }
            dropCardPC();
            if (statusPC == 3)
            {
                statusPC = 0;
                enabledAct();
                return;
            }
            else
            {
                statusPC = 0;
                cardActionPlayer(lastActPlayer);
                enabledAct();
                computerRun(isPCRUN);
                return;
            }
        }
        else
        {
            dropCardPlayer(2);
            statusPC = 0;
            enabledAct();
            computerRun(isPCRUN);
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
    lastActPlayer = "Предприниматель";
    lastUseAct = lastActPlayer;
    isPCRUN = true;
    if (checkingPC())
    {
        ui->statusPCText->setText("Проверяю");
        enabledAct();
        return;
    }
    else
    {
        cardActionPlayer(lastActPlayer);
        enabledAct();
        computerRun(isPCRUN);
        return;
    }
}

void GameWindow::on_reactButton_clicked()
{
    lastActPlayer = "Диссидент";
    ui->reactButton->setEnabled(false);
    statusPresentTV = false;
    return;
}

void GameWindow::on_redButton_clicked()
{
    QString str;
    lastActPlayer = "Бюрократ";
    lastUseAct = lastActPlayer;
    isPCRUN = true;
    if (checkingPC())
    {
        ui->statusPCText->setText("Проверяю");
        enabledAct();
        return;
    }
    else
    {
        cardActionPlayer(lastActPlayer);
        enabledAct();
        computerRun(isPCRUN);
        return;
    }
}

void GameWindow::on_yellowButton_clicked()
{
    QString str;
    lastActPlayer = "Телеведущий";
    lastUseAct = lastActPlayer;
    if (checkingPC())
    {
        statusPresentTV = false;
        ui->statusPCText->setText("Проверяю");
        enabledAct();
        return;
    }
    else
    {
        statusPresentTV = true;
        cardActionPlayer(lastActPlayer);
        enabledAct();
        return;
    }
}

void GameWindow::on_blueButton_clicked()
{
    QString str;
    lastActPlayer = "Киллер";
    lastUseAct = lastActPlayer;
    if ((lastActPC == "Киллер") && (ui->statusPCText->text() == "Киллер"))
    {
        statusPC = 0;
        cardMoney(lastActPC, player, playerPC);
        ui->moneyPCLabel->setText(str.setNum(playerPC->money));
        lastActPlayer = "Блокирую";
        if (checkingPC())
        {
            ui->statusPCText->setText("Проверяю");
            enabledAct();
            ui->reactButton->setEnabled(false);
            return;
        }
        else
        {
            ui->statusPCText->setText("");
            enabledAct();
            return;
        }
    }
    if (checkingPC())
    {
        ui->statusPCText->setText("Проверяю");
        enabledAct();
        return;
    }
    else
    {
        cardActionPlayer(lastActPlayer);
        enabledAct();
    }
}

void GameWindow::on_money1Button_clicked()
{
    QString str;
    lastActPlayer = "1 монета";
    isPCRUN = true;
    player->money++;
    ui->moneyLabel->setText(str.setNum(player->money));
    enabledAct();
    computerRun(isPCRUN);
    return;
}

void GameWindow::on_restructButton_clicked()
{
    QString str;
    lastActPlayer = "Перестройка";
    isPCRUN = true;
    player->money -= 7;
    ui->moneyLabel->setText(str.setNum(player->money));
    dropCardPC();
    enabledAct();
    computerRun(isPCRUN);
}

void GameWindow::on_checkButton_clicked()
{
    QString str;
    statusPC = 0;
    ui->statusPCText->setText("");
    if (lastActPC == "Предприниматель")
    {
        if (checkCardPC(lastActPC) == 1)
        {
            ui->card1PCLabel->setStyleSheet(cardColor(cards[2]));
            ui->shirtCard1PCButton->setVisible(false);
            useEndPredPC();
            statusPC = 1;
            enabledAct();
            return;
        }
        if (checkCardPC(lastActPC) == 2)
        {
            ui->card2PCLabel->setStyleSheet(cardColor(cards[3]));
            ui->shirtCard2PCButton->setVisible(false);
            useEndPredPC();
            statusPC = 1;
            enabledAct();
            return;
        }
        if (checkCardPC(lastActPC) == 0)
        {
            statusPC = 0;
            dropCardPC();
            enabledAct();
            return;
        }
    }
    if (lastActPC == "Киллер")
    {
        if (checkCardPC(lastActPC) == 1)
        {
            ui->card1PCLabel->setStyleSheet(cardColor(cards[2]));
            ui->shirtCard1PCButton->setVisible(false);
            if (ui->deathCard1Label->text() == "") dropCardPlayer(1);
            if (ui->deathCard2Label->text() == "") dropCardPlayer(2);
            enabledAct();
            return;
        }
        if (checkCardPC(lastActPC) == 2)
        {
            ui->card2PCLabel->setStyleSheet(cardColor(cards[3]));
            ui->shirtCard2PCButton->setVisible(false);
            if (ui->deathCard1Label->text() == "") dropCardPlayer(1);
            if (ui->deathCard2Label->text() == "") dropCardPlayer(2);
            enabledAct();
            return;
        }
        if (checkCardPC(lastActPC) == 0)
        {
            statusPC = 0;
            dropCardPC();
            enabledAct();
            return;
        }
    }
    if (lastActPC == "Телеведущий")
    {
        if (checkCardPC(lastActPC) == 1)
        {
            ui->card1PCLabel->setStyleSheet(cardColor(cards[2]));
            ui->shirtCard1PCButton->setVisible(false);
            usePresentTVPC();
            statusPC = 1;
            enabledAct();
            return;
        }
        if (checkCardPC(lastActPC) == 2)
        {
            ui->card2PCLabel->setStyleSheet(cardColor(cards[3]));
            ui->shirtCard2PCButton->setVisible(false);
            usePresentTVPC();
            statusPC = 1;
            enabledAct();
            return;
        }
        if (checkCardPC(lastActPC) == 0)
        {
            statusPC = 0;
            dropCardPC();
            enabledAct();
            return;
        }
    }
    if (lastActPC == "Бюрократ")
    {
        if (checkCardPC(lastActPC) == 1)
        {
            ui->card1PCLabel->setStyleSheet(cardColor(cards[2]));
            ui->shirtCard1PCButton->setVisible(false);
            useBurocratPC();
            statusPC = 1;
            enabledAct();
            return;
        }
        if (checkCardPC(lastActPC) == 2)
        {
            ui->card2PCLabel->setStyleSheet(cardColor(cards[3]));
            ui->shirtCard2PCButton->setVisible(false);
            useBurocratPC();
            statusPC = 1;
            enabledAct();
            return;
        }
        if (checkCardPC(lastActPC) == 0)
        {
            statusPC = 0;
            dropCardPC();
            enabledAct();
            return;
        }
    }
    if (lastActPC == "Диссидент")
    {
        if ((typeCardPC1 == "Диссидент") && (ui->card1PCLabel->isEnabled() == false))
        {
            ui->card1PCLabel->setStyleSheet(cardColor(cards[2]));
            ui->shirtCard1PCButton->setVisible(false);
            statusPC = 1;
            cardMoney(typeCardPC1, player, playerPC);
            ui->moneyPCLabel->setText(str.setNum(playerPC->money));
            enabledAct();
            return;
        }
        if ((typeCardPC2 == "Диссидент") && (ui->card2PCLabel->isEnabled() == false))
        {
            ui->card2PCLabel->setStyleSheet(cardColor(cards[3]));
            ui->shirtCard2PCButton->setVisible(false);
            statusPC = 1;
            cardMoney(typeCardPC2, player, playerPC);
            ui->moneyPCLabel->setText(str.setNum(playerPC->money));
            enabledAct();
            return;
        }
        if (checkCardPC("Диссидент") == 0)
        {
            ui->shirtCard1PCButton->setVisible(false);
            ui->shirtCard2PCButton->setVisible(false);
            statusPC = 0;
            dropCardPC();
            enabledAct();
            return;
        }
    }
    if (lastActPC == "Блокирую")
    {
        if ((checkCardPC("Киллер") == 1))
        {
            ui->card1PCLabel->setStyleSheet(cardColor(cards[2]));
            ui->shirtCard1PCButton->setVisible(false);
            useKillerPC();
            statusPC = 1;
            enabledAct();
            return;
        }
        if ((checkCardPC("Киллер") == 2))
        {
            ui->card2PCLabel->setStyleSheet(cardColor(cards[3]));
            ui->shirtCard2PCButton->setVisible(false);
            useKillerPC();
            statusPC = 1;
            enabledAct();
            return;
        }
        if (checkCardPC("Киллер") == 0)
        {
            if (player->count_card == 2)
            {
                ui->card1PCLabel->setStyleSheet(cardColor(cards[2]));
                ui->statusPCText->setText("");
                playerPC->count_card--;
                ui->card1PCLabel->setGeometry(ui->card1PCLabel->x(), ui->card1PCLabel->y() + 40, ui->card1PCLabel->width(), ui->card1PCLabel->height());
                ui->card1PCLabel->setEnabled(false);
                ui->deathCardPC1Label->setText("Мертвая карта");
                ui->shirtCard1PCButton->setVisible(false);

                ui->card2PCLabel->setStyleSheet(cardColor(cards[3]));
                ui->statusPCText->setText("");
                playerPC->count_card--;
                ui->card2PCLabel->setGeometry(ui->card2PCLabel->x(), ui->card2PCLabel->y() + 40, ui->card2PCLabel->width(), ui->card2PCLabel->height());
                ui->card2PCLabel->setEnabled(false);
                ui->deathCardPC2Label->setText("Мертвая карта");
                ui->shirtCard2PCButton->setVisible(false);
                enabledAct();
                return;
            }
            else
            {
                if (ui->card1PCLabel->isEnabled())
                {
                    ui->card1PCLabel->setStyleSheet(cardColor(cards[2]));
                    ui->statusPCText->setText("");
                    playerPC->count_card--;
                    ui->card1PCLabel->setGeometry(ui->card1PCLabel->x(), ui->card1PCLabel->y() + 40, ui->card1PCLabel->width(), ui->card1PCLabel->height());
                    ui->card1PCLabel->setEnabled(false);
                    ui->deathCardPC1Label->setText("Мертвая карта");
                    ui->shirtCard1PCButton->setVisible(false);
                    enabledAct();
                    return;
                }
                if (ui->card2PCLabel->isEnabled())
                {
                    ui->card2PCLabel->setStyleSheet(cardColor(cards[3]));
                    ui->statusPCText->setText("");
                    playerPC->count_card--;
                    ui->card2PCLabel->setGeometry(ui->card2PCLabel->x(), ui->card2PCLabel->y() + 40, ui->card2PCLabel->width(), ui->card2PCLabel->height());
                    ui->card2PCLabel->setEnabled(false);
                    ui->deathCardPC2Label->setText("Мертвая карта");
                    ui->shirtCard2PCButton->setVisible(false);
                    enabledAct();
                    return;
                }
            }
        }
    }
}

void GameWindow::on_not_checkButton_clicked()
{
    QString str;
    statusPC = 0;
    lastActPlayer = "Не проверяю";
    ui->statusPCText->setText("");
    if (lastActPC == "Предприниматель")
    {
        useEndPredPC();
        enabledAct();
        return;
    }
    if (lastActPC == "Киллер")
    {
        useKillerPC();
        enabledAct();
        return;
    }
    if (lastActPC == "Телеведущий")
    {
        usePresentTVPC();
        enabledAct();
        return;
    }
    if (lastActPC == "Бюрократ")
    {
        useBurocratPC();
        enabledAct();
        return;
    }
    if (lastActPC == "Диссидент")
    {
        if (lastActPlayer == "Телеведущий") cardActionPlayer(lastActPlayer);
        cardMoney("Диссидент", player, playerPC);
        ui->moneyPCLabel->setText(str.setNum(player->money));
        enabledAct();
        return;
    }
    if (lastActPC == "Блокирую")
    {
        enabledAct();
        computerRun(isPCRUN);
        return;
    }
}

void GameWindow::on_returnToDeckButton_clicked()
{
    isPCRUN = true;
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
    computerRun(isPCRUN);
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
