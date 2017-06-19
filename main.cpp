#include "includes.h"
#include "events.h"


int main(int argc, char *argv[])
{
    //Reajusta el tamaño de los elementos
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR","1");

    QApplication a(argc, argv);

    //Soporte para pantallas retina
    a.setAttribute(Qt::AA_EnableHighDpiScaling,true);

    //Instala la clase abstracta de eventos nativos de X11
    a.installNativeEventFilter(new Events());

    //Le dice al server que eventos quieres escuchar
    XSelectInput(QX11Info::display(), QX11Info::appRootWindow(QX11Info::appScreen()),
                 KeyPressMask |
                 KeyReleaseMask |
                 ButtonPressMask |
                 ButtonReleaseMask |
                 KeymapStateMask |
                 ButtonMotionMask |
                 PointerMotionMask |
                 EnterWindowMask |
                 LeaveWindowMask |
                 FocusChangeMask |
                 VisibilityChangeMask |
                 ExposureMask |
                 StructureNotifyMask |
                 SubstructureRedirectMask |
                 SubstructureNotifyMask
                 );

    //Inicia una terminal por defecto
    QProcess::startDetached("gnome-terminal");

    return a.exec();
}
