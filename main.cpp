#include "includes.h"
#include "events.h"


int main(int argc, char *argv[])
{
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR","1");

    QApplication a(argc, argv);
    a.setAttribute(Qt::AA_EnableHighDpiScaling);


    QWidget *background = new QWidget();
    background->setGeometry(a.desktop()->geometry());
    background->setStyleSheet("background:white");
    background->show();
    a.installNativeEventFilter(new Events());

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

    QProcess::startDetached("gnome-terminal");
    return a.exec();
}
