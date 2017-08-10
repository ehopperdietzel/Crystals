#include "headers/headers.h"
#include "headers/compositor.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    Compositor *compositor = new Compositor();

    return app.exec();
}
