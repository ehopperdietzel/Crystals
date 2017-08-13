#include "titlebar.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TitleBar w;
    w.show();

    return a.exec();
}
