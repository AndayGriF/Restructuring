#ifndef STARTGAME_H
#define STARTGAME_H

#include <QMainWindow>
#include "game.h"

namespace Ui {
class StartGame;
}

class StartGame : public QMainWindow
{
    Q_OBJECT

public:
    explicit StartGame(QWidget *parent = nullptr);
    ~StartGame();

signals:
    void firstWindow();

private slots:
    void on_MainMenu_clicked();

private:
    Ui::StartGame *ui;
};

#endif // STARTGAME_H
