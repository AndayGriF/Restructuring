#ifndef SETTINGS_H
#define SETTINGS_H

#include <QMainWindow>

namespace Ui {
class Settings;
}

class Settings : public QMainWindow
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

private slots:

    void on_MainMenu_clicked();

private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H
