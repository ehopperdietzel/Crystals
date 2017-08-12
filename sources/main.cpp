#include "headers/headers.h"
#include "headers/compositor.h"


int main(int argc, char *argv[])
{
    // Non Widgets Application
    QGuiApplication app(argc, argv);

    // Creates the Compositor
    Compositor *compositor = new Compositor();

    // Execute Application
    return app.exec();
}
