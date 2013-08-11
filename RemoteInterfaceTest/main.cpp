#include "RemoteInterfaceTest.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RemoteInterfaceTest w;
    w.show();
    return a.exec();
}
