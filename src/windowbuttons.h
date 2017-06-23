#ifndef WINDOWBUTTONS_H
#define WINDOWBUTTONS_H

#include "includes.h"

//Clase de boton

class TopBarButton : public QPushButton
{
    Q_OBJECT
public:
    QIcon normal,active;
    QGraphicsColorizeEffect *effect = new QGraphicsColorizeEffect(this);
    TopBarButton(QString normalPix,QString activePix,QSize size)
    {
        effect->setEnabled(false);
        effect->setColor(QColor(Qt::black));
        effect->setStrength(0.4);
        setMouseTracking(true);
        normal = QIcon(normalPix);
        active = QIcon(activePix);
        setIconSize(size);
        setFixedSize(size);
        setIcon(normal);
        setStyleSheet("border:none;outline:none");
        setGraphicsEffect(effect);
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
    explicit WindowButtons()
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

    //Botones
    TopBarButton *closeButton = new TopBarButton(":/img/window/Buttons/w_close_n.png",":/img/window/Buttons/w_close_h.png",QSize(11,11));
    TopBarButton *minimizeButton = new TopBarButton(":/img/window/Buttons/w_minimize_n.png",":/img/window/Buttons/w_minimize_h.png",QSize(11,11));
    TopBarButton *expandButton = new TopBarButton(":/img/window/Buttons/w_expand_n.png",":/img/window/Buttons/w_expand_h.png",QSize(11,11));

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
