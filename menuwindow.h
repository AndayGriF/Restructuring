#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QMainWindow>
#include "settingswindow.h"
#include "flatwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MenuWindow; }
QT_END_NAMESPACE

class MenuWindow : public QMainWindow
{
    Q_OBJECT

public:
    MenuWindow(QWidget *parent = nullptr);
    ~MenuWindow();

private slots:
    void on_startButton_clicked();

    void on_settingsButton_clicked();

    void on_exitButton_clicked();

private:
    Ui::MenuWindow *ui;
    FlatWindow *FlatWin;
    SettingsWindow *SetWin;

};
#endif // MENUWINDOW_H
