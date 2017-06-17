#include "includes.h"

int main(int argc, char *argv[])
{
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR","1");
    QApplication a(argc, argv);
    a.setAttribute(Qt::AA_EnableHighDpiScaling);
    QWidget w;
    w.show();
    QLabel *background = new QLabel();
    background->setPixmap(QPixmap("/home/eduardo/Downloads/wallpaper.jpg"));
    background->setScaledContents(true);
    background->setGeometry(a.desktop()->geometry());
    background->show();
    QProcess::startDetached(a.applicationDirPath() + "/../crystals-b/./Crystals " + QString::number(background->winId()));
    return a.exec();
}
