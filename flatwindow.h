#ifndef FLATWINDOW_H
#define FLATWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "gamewindow.h"

namespace Ui {
class FlatWindow;
}

class FlatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FlatWindow(QWidget *parent = nullptr);
    ~FlatWindow();

signals:
    void firstwindow();

private slots:
    void on_backmenuBatton_clicked();

    void on_addPlayerButton_clicked();

    void on_deletePlayerButton_clicked();

    void on_startGameButton_clicked();

private:
    Ui::FlatWindow *ui;
    GameWindow *GameWin;
};

#endif // FLATWINDOW_H
