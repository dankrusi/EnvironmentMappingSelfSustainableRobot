#include "DifferentialSteeringTest.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DifferentialSteeringTest w;
    w.show();
    return a.exec();
}
