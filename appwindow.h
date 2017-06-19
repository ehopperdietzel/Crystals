#ifndef APPWINDOW_H
#define APPWINDOW_H

#include "includes.h"
#include "topbarbutton.h"

//Esta clase es para crear contenedores ( Ventanas ) para las aplicaciones graficas que se ejecuten.

class AppWindow : public QFrame
{
public:
    AppWindow(xcb_window_t _client);

    //Separa la ventana del topbar
    QBoxLayout *mainLayout = new QBoxLayout(QBoxLayout::TopToBottom,this);

    //Ventana cliente
    QWindow *win;

    //Contenedor de la ventana
    QWidget *window = new QWidget();

    //Elementos del top bar
    QFrame *topBar = new QFrame();
    QLabel *title = new QLabel();
    TopBarButton *cloBtn = new TopBarButton(":/img/window/cloWin.png",":/img/window/cloWin_A.png",QSize(10,10));
    TopBarButton *minBtn = new TopBarButton(":/img/window/minWin.png",":/img/window/minWin_A.png",QSize(10,10));
    TopBarButton *expBtn = new TopBarButton(":/img/window/expWin.png",":/img/window/expWin_A.png",QSize(10,10));
    QBoxLayout *topBarLayout = new QBoxLayout(QBoxLayout::LeftToRight,topBar);

    //Win ID del cliente
    int client;

    //Almacena variables para mover la ventana
    QPoint prevWin;
    QPoint prevCur;
    bool pressed = false;

    void style();
    void resizeEvent(QResizeEvent*);
    void setWidgetBorderRadius(QWidget *,int radius);
    bool eventFilter(QObject *watched, QEvent *event);

public slots:
    void closeWindow();
};

#endif // APPWINDOW_H
