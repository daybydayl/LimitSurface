#include "CMainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CMainWindow w;
    w.resize(1400, 900);
    w.show();
    return a.exec();
}
