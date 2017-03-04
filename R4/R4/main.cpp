#include "R4GLWidget.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    R4GLWidget w;
    w.show();
    return a.exec();
}
