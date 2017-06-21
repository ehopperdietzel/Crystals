#ifndef HUINCHA_H
#define HUINCHA_H

#include <QApplication>
#include <QScreen>

class Huincha{
public:

    //Retornan la cantidad de pixeles contenidos en un determinado porciento de la pantalla.

    float relativeWidth(float percent)
    {
        QSize screen = QApplication::primaryScreen()->geometry().size();
        qreal dpi = QGuiApplication::primaryScreen()->logicalDotsPerInch();
        return (float)screen.width()*dpi / (float)100.0 * percent;
    }

    float relativeHeight(float percent)
    {
        QSize screen = QApplication::primaryScreen()->geometry().size();
        qreal dpi = QGuiApplication::primaryScreen()->devicePixelRatio();
        return ((float)screen.height() / (float)100.0 * percent)/dpi;
    }

    QSize relativeSize(float widthPercent,float heightPercent)
    {
        QSize screen = QApplication::primaryScreen()->geometry().size();
        qreal dpi = QGuiApplication::primaryScreen()->logicalDotsPerInch();

        QSize newSize((float)screen.width()*dpi / (float)100.0 * widthPercent,
                      (float)screen.height()*dpi / (float)100.0 * heightPercent);

        return newSize;
    }

};

#endif // HUINCHA_H
