#ifndef WINDOW_H
#define WINDOW_H

#include "capplication.h"
#include <QBoxLayout>
#include <QPushButton>
#include <QTimer>

class Window : public CWidget
{
    Q_OBJECT
public:
    Window();
    QPushButton *mov = new QPushButton("Mover");
    QPushButton *cre = new QPushButton("Crear");
    QPushButton *opa = new QPushButton("Opacidad");
    QPushButton *mod = new QPushButton("Modo");
    QPushButton *blu = new QPushButton("Blur");
    QPushButton *tit = new QPushButton("Title");
    QPushButton *siz = new QPushButton("Resize");
    QPushButton *del = new QPushButton("Delete");
    QHBoxLayout *layout = new QHBoxLayout(this);
    QTimer *time = new QTimer(this);
    bool movee = false;
    void mouseMoveEvent(QMouseEvent *event)
    {
        if(movee)
        move(QCursor::pos()+QPoint(-100,-100));
    }

public slots:
    void moveWindow();
    void deleteWindow();
    void createWindow();
    void activateBlur();
    void changeOpacity();
    void changeMode();
    void changeTitle();
    void changeSize();
    void timeStopped();
};

#endif // WINDOW_H
