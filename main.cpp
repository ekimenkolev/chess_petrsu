#include "chess.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    chess ggame;

    ggame.show();

    return a.exec();
}
