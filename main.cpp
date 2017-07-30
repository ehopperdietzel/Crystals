#include "headers.h"
#include "window.h"
#include "compositor.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Window window;
    Compositor compositor(&window);
    window.setCompositor(&compositor);
    compositor.create();
    window.resize(800,600);
    window.show();

    return app.exec();
}
