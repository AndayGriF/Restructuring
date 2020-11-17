#include "menuwindow.h"
#include "gamewindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MenuWindow w;
    GameWindow g;
    g.show();
    return a.exec();
}
