#ifndef STARTGAME_H
#define STARTGAME_H

#include <QMainWindow>

namespace Ui {
class StartGame;
}

class StartGame : public QMainWindow
{
    Q_OBJECT

public:
    explicit StartGame(QWidget *parent = nullptr);
    ~StartGame();

private:
    Ui::StartGame *ui;
};

#endif // STARTGAME_H
