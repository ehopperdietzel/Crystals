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
        return (float)screen.width() / (float)100.0 * percent;
    }

    float relativeHeight(float percent)
    {
        QSize screen = QApplication::primaryScreen()->geometry().size();
        qreal dpi = QGuiApplication::primaryScreen()->devicePixelRatio();
        return ((float)screen.height() / (float)100.0 * percent);
    }

    QSize relativeSize(float widthPercent,float heightPercent)
    {
        QSize screen = QApplication::primaryScreen()->geometry().size();
        qreal dpi = QGuiApplication::primaryScreen()->logicalDotsPerInch();

        QSize newSize((float)screen.width() / (float)100.0 * widthPercent,
                      (float)screen.height() / (float)100.0 * heightPercent);

        return newSize;
    }

    //Retornan el porcentaje de pantalla relacionado a la cantidad de pixeles del argumento

    float getPercentWidth(float pixels)
    {
        QSize screen = QApplication::primaryScreen()->geometry().size();
        qreal dpi = QGuiApplication::primaryScreen()->logicalDotsPerInch();
        return (float)100.0 / (float)screen.width() * pixels;
    }

    float getPercentHeight(float pixels)
    {
        QSize screen = QApplication::primaryScreen()->geometry().size();
        qreal dpi = QGuiApplication::primaryScreen()->devicePixelRatio();
        return (float)100.0 / (float)screen.height() * pixels;
    }

};

#endif // HUINCHA_H
