#include "erikis.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Erikis w;
    w.show();
    return a.exec();
}
