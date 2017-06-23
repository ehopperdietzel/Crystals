#include "includes.h"
#include "events.h"


int main(int argc, char *argv[])
{
    //Ajusta la densidad de pixeles
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR","1");

    QApplication a(argc, argv);


    //Identificador para QSettings
    a.setOrganizationName("CuarzoSoftware");
    a.setApplicationName("Crystals");

    //Soporte para pantallas retina
    a.setAttribute(Qt::AA_UseHighDpiPixmaps);
    a.setAttribute(Qt::AA_EnableHighDpiScaling);


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
    return a.exec();
}
