#include "player.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Player mainwindow;
    mainwindow.show();

    return a.exec();
}
