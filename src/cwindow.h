#ifndef APPWINDOW_H
#define APPWINDOW_H

#include "includes.h"
#include "huincha.h"
#include "topbar.h"


//Esta clase es para crear contenedores ( Ventanas ) para las aplicaciones graficas que se ejecuten.

class CWindow : public QLabel
{
    Q_OBJECT
public:

    //Constructores

    CWindow(int _client);

    // ORDEN ---------------------------------
    // AppWindow -> Container -> Topbar
    // ------------------------  Window ------

    //Crea un espacio para las sombras
    QBoxLayout *mainLayout = new QBoxLayout(QBoxLayout::TopToBottom,this);

    //Contiene la parte visible de la ventana
    QFrame *container = new QFrame();

    //Separa la ventana del topbar
    QBoxLayout *containerLayout = new QBoxLayout(QBoxLayout::TopToBottom,container);

    //Ventana Cliente
    QWindow *cWindow;

    //Contenedor de la ventana
    QWidget *window;

    //Barra de titulo
    TopBar *titleBar = new TopBar();

    //Sombra
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(container);

    //Opacidad
    QGraphicsOpacityEffect *opacity = new QGraphicsOpacityEffect(this);

    //Utilidades
    Huincha huincha;

    //Datos Cliente
    int client;
    QRect clientGeometry;
    QString clientName;

    //Datos de la pantalla
    float Xdpi = QX11Info::appDpiX();
    float Ydpi = QX11Info::appDpiY();

    QSize perSize;
    QPoint perPos;
    QString grabbingFrom = "";

    //Almacena variables para mover la ventana
    QPoint prevWin;
    QPoint prevCur;
    QSize  prevSize;
    bool pressed = false;

    //Eventos del cliente
    void resizeReq(int w, int h);
    void style();
    void resizeEvent(QResizeEvent*);
    void paintEvent(QPaintEvent*);
    bool eventFilter(QObject *watched, QEvent *event);

    //Getters
    void getClientGeometry();
    void getClientTitle();

public slots:
    void closeWindow();
};

#endif // APPWINDOW_H
