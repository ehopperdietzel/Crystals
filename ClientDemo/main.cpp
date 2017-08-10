#include "capplication.h"
#include "window.h"
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    CApplication *app = new CApplication(argc, argv);
    QFontDatabase::addApplicationFont("/home/e/font.ttf");
    Window *w = new Window();


    return app->exec();
}
