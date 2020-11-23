#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <iostream>
#include <stdio.h>
#include <QMainWindow>
#include <QMessageBox>

namespace Ui { class GameWindow; }

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

private slots:
    void on_changeButton_clicked();

    void on_card1Button_clicked();

    void on_card2Button_clicked();

private:
    Ui::GameWindow *ui;
};

#endif // GAMEWINDOW_H
