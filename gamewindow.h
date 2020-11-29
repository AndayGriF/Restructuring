#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <iostream>
#include <stdio.h>
#include <QMainWindow>
#include <QMessageBox>

typedef struct
{
    int money;
    int count_card;
} Player;

namespace Ui { class GameWindow; }

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

private slots:

    void enabledButtons(bool b);

    void cardMoney(QString typeCard, Player *playerFrom, Player *playerTo);

    void dropCardPC();

    void enabledAct();

    void on_changeButton_clicked();

    void on_card1Button_clicked();

    void on_card2Button_clicked();

    void on_greenButton_clicked();

    void on_reactButton_clicked();

    void on_redButton_clicked();

    void on_yellowButton_clicked();

    void on_blueButton_clicked();

    void on_money1Button_clicked();

    void on_restructButton_clicked();

    void on_checkButton_clicked();

    void on_returnToDeckButton_clicked();

private:
    Ui::GameWindow *ui;
};

#endif // GAMEWINDOW_H
