#ifndef APPWINDOW_H
#define APPWINDOW_H

#include "includes.h"
#include "huincha.h"
#include "appwindowtitlebar.h"


//Esta clase es para crear contenedores ( Ventanas ) para las aplicaciones graficas que se ejecuten.

class AppWindow : public QWidget
{
public:
    AppWindow(xcb_window_t _client);

    // ORDEN ---------------------------------
    // AppWindow -> Container -> Topbar -> Win
    // ------------------------  Window ------

    //Crea un espacio para las sombras
    QBoxLayout *mainLayout = new QBoxLayout(QBoxLayout::TopToBottom,this);

    //Contiene la parte visible de la ventana
    QFrame *container = new QFrame();

    //Separa la ventana del topbar
    QBoxLayout *containerLayout = new QBoxLayout(QBoxLayout::TopToBottom,container);

    //Ventana cliente
    QWindow *win;

    //Contenedor de la ventana
    QWidget *window = new QWidget();

    //Barra de titulo
    AppWindowTitleBar *titleBar = new AppWindowTitleBar();

    //Sombra
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(container);

    //Utilidades
    Huincha huincha;

    //Win ID del cliente
    int client;
    QSize perSize;
    QPoint perPos;

    //Almacena variables para mover la ventana
    QPoint prevWin;
    QPoint prevCur;
    bool pressed = false;

    //Eventos del cliente
    void resizeReq(int w, int h);

    void style();
    void resizeEvent(QResizeEvent*);
    bool eventFilter(QObject *watched, QEvent *event);

public slots:
    void closeWindow();
};

#endif // APPWINDOW_H
