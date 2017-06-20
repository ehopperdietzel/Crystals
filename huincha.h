#ifndef HUINCHA_H
#define HUINCHA_H

#include <QApplication>
#include <QScreen>

class Huincha{
public:

    //Retornan la cantidad de pixeles contenidos en un determinado porciento de la pantalla.

    float relativeWidth(float percent)
    {
        QRect screen = QApplication::primaryScreen()->geometry();
        return (float)screen.width() / 100.0 * percent;
    }

    float relativeHeight(float percent)
    {
        QRect screen = QApplication::primaryScreen()->geometry();
        return (float)screen.height() / 100.0 * percent;
    }

    QSize relativeSize(float widthPercent,float heightPercent)
    {
        QRect screen = QApplication::primaryScreen()->geometry();

        QSize newSize((float)screen.width() / 100.0 * widthPercent,
                      (float)screen.height() / 100.0 * heightPercent);

        return newSize;
    }

};

#endif // HUINCHA_H
