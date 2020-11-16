#ifndef FLATWINDOW_H
#define FLATWINDOW_H

#include <QMainWindow>

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

private:
    Ui::FlatWindow *ui;
};

#endif // FLATWINDOW_H
