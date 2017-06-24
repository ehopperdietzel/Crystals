#ifndef WINDOWBUTTONS_H
#define WINDOWBUTTONS_H

#include "includes.h"

//Clase de boton

class TopBarButton : public QToolButton
{
    Q_OBJECT
public:
    QIcon normal,active;
    QSize size;
    QGraphicsColorizeEffect *effect = new QGraphicsColorizeEffect(this);
    TopBarButton(QString normalPix,QString activePix,QSize _size)
    {
        size = _size;
        setStyleSheet("border:none;outline:none");
        setMouseTracking(true);
        normal = QIcon(normalPix);
        active = QIcon(activePix);
        setIconSize(size);
        setFixedSize(size);
        effect->setColor(Qt::black);
        effect->setStrength(0.7);
        effect->setEnabled(false);
        setGraphicsEffect(effect);
        setIcon(normal);
    }

    void mousePressEvent(QMouseEvent*)
    {
        effect->setEnabled(true);
    }

    void mouseReleaseEvent(QMouseEvent*)
    {
        effect->setEnabled(false);
    }

};

//Contenedor de los botones de topbar

class WindowButtons : public QWidget
{
    Q_OBJECT
public:
    WindowButtons()
    {
        setAttribute(Qt::WA_Hover);

        layout->addWidget(closeButton);
        layout->addWidget(minimizeButton);
        layout->addWidget(expandButton);

        layout->setMargin(0);
        layout->setSpacing(8);
    }

    //Layout
    QHBoxLayout *layout = new QHBoxLayout(this);

    //Tamaño de botones
    QSize bSize = QSize(12,12);
    //Botones
    TopBarButton *closeButton = new TopBarButton(":/img/window/Buttons/w_close_n.png",":/img/window/Buttons/w_close_h.png",bSize);
    TopBarButton *minimizeButton = new TopBarButton(":/img/window/Buttons/w_minimize_n.png",":/img/window/Buttons/w_minimize_h.png",bSize);
    TopBarButton *expandButton = new TopBarButton(":/img/window/Buttons/w_expand_n.png",":/img/window/Buttons/w_expand_h.png",bSize);

    void enterEvent(QEvent *)
    {
        closeButton->setIcon(closeButton->active);
        minimizeButton->setIcon(minimizeButton->active);
        expandButton->setIcon(expandButton->active);
    }

    void leaveEvent(QEvent *)
    {
        closeButton->setIcon(closeButton->normal);
        minimizeButton->setIcon(minimizeButton->normal);
        expandButton->setIcon(expandButton->normal);
    }

signals:

public slots:
};




#endif // WINDOWBUTTONS_H
