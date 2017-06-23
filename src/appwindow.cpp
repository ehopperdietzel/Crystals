#include "appwindow.h"

AppWindow::AppWindow(xcb_window_t _client)
{

    //Almacena el WinID para usos futuros
    client = _client;

    //Le dice al server que eventos quieres escuchar
    XSelectInput(QX11Info::display(),window->winId(), KeyPressMask | KeyReleaseMask |  ButtonPressMask | ButtonReleaseMask | KeymapStateMask | ButtonMotionMask | PointerMotionMask |  EnterWindowMask | LeaveWindowMask |  FocusChangeMask |  VisibilityChangeMask |  ExposureMask | StructureNotifyMask | SubstructureRedirectMask | SubstructureNotifyMask );

    //Mapea el cliente en un QWindow
    win = QWindow::fromWinId(client);

    //Se crea un conetedor para el QWindow
    window = QWidget::createWindowContainer(win);

    //Obtienes los atributos del cliente y se almacenan en attr.
    XWindowAttributes attr;

    XGetWindowAttributes(QX11Info::display(), client, &attr);

    int _x = attr.x;
    int _y = attr.y;
    int _w = attr.width;
    int _h = attr.height;

    if(_w < 300 || _h < 300)
    {
        QSize sSize = QApplication::desktop()->size();
        window->setFixedSize(sSize.width()/2,sSize.height()/2);
    }
    else
    {
        window->setFixedSize(_w/QApplication::primaryScreen()->devicePixelRatio(),_h/QApplication::primaryScreen()->devicePixelRatio());
    }

    move(_x,_y + 30);

    //XMoveResizeWindow(QX11Info::display(),client,0,0,_w,_h);

    //Configura los estilos
    style();

}

AppWindow::AppWindow(int _client, int x, int y, int w, int h)
{

    //Almacena el WinID para usos futuros
    client = _client;

    //Le dice al server que eventos quieres escuchar
    XSelectInput(QX11Info::display(),window->winId(), KeyPressMask | KeyReleaseMask |  ButtonPressMask | ButtonReleaseMask | KeymapStateMask | ButtonMotionMask | PointerMotionMask |  EnterWindowMask | LeaveWindowMask |  FocusChangeMask |  VisibilityChangeMask |  ExposureMask | StructureNotifyMask | SubstructureRedirectMask | SubstructureNotifyMask );

    //Mapea el cliente en un QWindow
    win = QWindow::fromWinId(client);

    //Se crea un conetedor para el QWindow
    window = QWidget::createWindowContainer(win);

    //Asigna las proporciones

    window->setFixedSize(w,h);
    move(x,y);

    //Configura los estilos
    style();

}



void AppWindow::style() //Añade los estilos a todos los elementos
{
    //Fondo transparente
    setAttribute(Qt::WA_TranslucentBackground);

    //Obtener eventos
    installEventFilter(this);
    titleBar->installEventFilter(this);

    //Ajusta el layout principal, con margin para que se vea el drop shadow
    mainLayout->addWidget(container);
    mainLayout->setContentsMargins(8,4,8,8);

    //Ajusta el layout que separa la barra de titulo con la ventana
    containerLayout->addWidget(titleBar);
    containerLayout->addWidget(window,1);
    containerLayout->setMargin(0);
    containerLayout->setSpacing(0);


    //Añade fondo a la ventana visible
    container->setObjectName("WD");

    //Estilo de la ventana
    container->setStyleSheet("#WD{background:#FAFAFA;border-top-left-radius:7px;border-top-right-radius:7px;}");

    //Crea la sombra
    shadow->setColor(QColor(0,0,0,40));
    shadow->setBlurRadius(16);
    shadow->setOffset(0,0);
    container->setGraphicsEffect(shadow);

    //Muestra la ventana
    show();
}


void AppWindow::resizeReq(int w, int h) //El cliente desea cambiar de tamaño
{
    if(w < 300){
        container->setFixedWidth(300);
    }
    else{
        container->setFixedWidth(w);
    }

    if(h < 300){
        container->setFixedHeight(300 + 30);
    }
    else{
        container->setFixedHeight(w + 30);
    }
}


void AppWindow::resizeEvent(QResizeEvent *) //Evento cuando la ventana cambia de tamaño.
{

}


void AppWindow::closeWindow()
{
    delete this;
}


bool AppWindow::eventFilter(QObject *watched, QEvent *event)
{

    if(event->type() == QEvent::HoverEnter)
    {
        //blabla
    }

    if(event->type() == QEvent::MouseButtonPress)
    {
        //Eleva la ventana
        raise();

        if( (watched == titleBar ) && !pressed)
        {
            QMouseEvent *me = static_cast<QMouseEvent*>(event);
            Q_UNUSED(me); // Para detener las alertas de NO uso.
            prevCur = QCursor::pos();
            prevWin = pos();
            pressed = true;
            return true;
        }
    }

    if(event->type() == QEvent::MouseButtonRelease)
    {
        pressed = false;
        return true;
    }

    if(event->type() == QEvent::MouseMove)
    {
        if(pressed)
        {
            QMouseEvent *me = static_cast<QMouseEvent*>(event);
            Q_UNUSED(me); // Para detener las alertas de NO uso.
            QPoint newPos = QCursor::pos() - prevCur + prevWin;
            move(newPos);
            return true;
        }
    }

    return false;
}

