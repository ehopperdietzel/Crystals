#include "cwindow.h"

CWindow::CWindow(int _client)
{
    //Almacena el WinID para usos futuros
    client = _client;

    //Para poder obtener algunos eventos del cliente
    XSetWindowAttributes at;
    at.event_mask = XCB_EVENT_MASK_STRUCTURE_NOTIFY|XCB_EVENT_MASK_EXPOSURE|XCB_EVENT_MASK_RESIZE_REDIRECT|XCB_EVENT_MASK_PROPERTY_CHANGE;
    XChangeWindowAttributes(QX11Info::display(), client, CWEventMask, &at);

    //Obtiene las dimensiones del cliente
    getClientGeometry();
    getClientTitle();

    //Mapea la ventana
    cWindow = QWindow::fromWinId(client);

    //Mapea al cliente
    window = QWidget::createWindowContainer(cWindow);

    move(clientGeometry.x(),clientGeometry.y() + titleBar->height());

    setFixedSize(clientGeometry.width(),clientGeometry.height() +  titleBar->height());

    window->move(0,0);
    cWindow->setPosition(0,0);

    //Configura los estilos
    style();

}


void CWindow::style() //Añade los estilos a todos los elementos
{

    titleBar->title->setText(clientName);

    //Fondo transparente
    setAttribute(Qt::WA_TranslucentBackground);
    opacity->setEnabled(false);
    setGraphicsEffect(opacity);

    //Obtener eventos
    setAttribute(Qt::WA_Hover);
    setMouseTracking(true);
    installEventFilter(this);
    titleBar->setMouseTracking(true);
    titleBar->installEventFilter(this);

    //Ajusta el layout principal, con margin para que se vea el drop shadow
    mainLayout->addWidget(container);
    mainLayout->setContentsMargins(8,4,8,8);

    //Ajusta el layout que separa la barra de titulo con la ventana
    containerLayout->addWidget(titleBar,0);
    containerLayout->addWidget(window,10);
    containerLayout->setMargin(0);
    containerLayout->setSpacing(0);


    //Estilo de la ventana
    container->setObjectName("Container");
    container->setStyleSheet("#Container{background:#FFF;border-radius:2px}");

    //Tamaños
    setMaximumSize(QApplication::desktop()->size());
    setMinimumSize(128,128);

    //Crea la sombra
    shadow->setColor(QColor(0,0,0,100));
    shadow->setBlurRadius(16);
    shadow->setOffset(0,0);
    container->setGraphicsEffect(shadow);

    //Muestra la ventana
    show();
}


void CWindow::resizeReq(int w, int h) //El cliente desea cambiar de tamaño
{
    //setFixedSize(w,h+titleBar->height());
}


void CWindow::resizeEvent(QResizeEvent *) //Evento cuando la ventana cambia de tamaño.
{


}

void CWindow::paintEvent(QPaintEvent *)
{
    cWindow->setMask(QRegion(0,0,window->rect().width()-1,window->rect().height()));
}


void CWindow::closeWindow()
{
    deleteLater();
}


bool CWindow::eventFilter(QObject *watched, QEvent *event)
{

    if(event->type() == QEvent::HoverEnter)
    {

    }

    if(event->type() == QEvent::MouseButtonPress)
    {
        //Eleva la ventana
        raise();

        if(!pressed)
        {
            QMouseEvent *me = static_cast<QMouseEvent*>(event);
            prevCur = QCursor::pos();
            prevWin = pos();
            prevSize = size();
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

        QPoint mouse = QCursor::pos();
        QPoint TL = geometry().topLeft();
        QPoint TR = geometry().topRight();
        QPoint BR = geometry().bottomRight();
        QPoint BL = geometry().bottomLeft();

        //Move
        if(pressed && grabbingFrom == "TB")
        {
            QMouseEvent *me = static_cast<QMouseEvent*>(event);
            QPoint newPos = mouse - prevCur + prevWin;
            move(newPos);
            return true;
        }

        //Resize Right
        if(pressed && grabbingFrom == "R")
        {
            QMouseEvent *me = static_cast<QMouseEvent*>(event);
            int newWidth = mouse.x() - prevCur.x() + prevSize.width();
            setFixedWidth(newWidth);
            XResizeWindow(QX11Info::display(),client,newWidth*Xdpi,window->height()*Ydpi);
            return true;
        }
        //Resize Right
        else if(pressed && grabbingFrom == "BR")
        {
            QMouseEvent *me = static_cast<QMouseEvent*>(event);
            int newWidth = mouse.x() - prevCur.x() + prevSize.width();
            int newHeight = mouse.y() - prevCur.y() + prevSize.height();
            //XResizeWindow(QX11Info::display(),client,newWidth*Xdpi,window->height());
            setFixedSize(newWidth,newHeight);
            return true;
        }

        //MouseOver
        if(watched == titleBar)
        {
            setCursor(Qt::ArrowCursor);
            grabbingFrom = "TB";
            return true;
        }
        else if(watched == this)
        {

            //Derecha
            if(mouse.x() >= BR.x() - 8 && mouse.y() > TR.y() + 4 && mouse.y() < BR.y() - 8)
            {
                grabbingFrom = "R";
                setCursor(Qt::SizeHorCursor);
                return true;
            }
            //Izquierda
            else if(mouse.x() <= BL.x() + 8 && mouse.y() > TR.y() + 4 && mouse.y() < BR.y() - 8)
            {
                grabbingFrom = "L";
                setCursor(Qt::SizeHorCursor);
                return true;
            }
            //Arriba Derecha
            else if(mouse.x() >= BR.x() - 8 && mouse.y() <= TR.y() + 4)
            {
                grabbingFrom = "TR";
                setCursor(Qt::SizeBDiagCursor);
                return true;
            }
            //Abajo Derecha
            else if(mouse.x() >= BR.x() - 8 && mouse.y() >= BR.y() - 8)
            {
                grabbingFrom = "BR";
                setCursor(Qt::SizeFDiagCursor);
                return true;
            }
            //Abajo
            else if(mouse.x() > BL.x() + 8 && mouse.x() < BR.x() - 8 && mouse.y() > BR.y() - 8)
            {
                grabbingFrom = "B";
                setCursor(Qt::SizeVerCursor);
                return true;
            }
            //Arriba
            else if(mouse.x() > BL.x() + 8 && mouse.x() < BR.x() - 8 && mouse.y() <= TR.y() + 4)
            {
                grabbingFrom = "T";
                setCursor(Qt::SizeVerCursor);
                return true;
            }

        }
    }

    return false;
}



/////////////GETTERS////////////

//Obtienes las dimensiones del cliente

void CWindow::getClientGeometry()
{
    int x,y;
    unsigned int w,h,bx,by;
    Window win;
    XGetGeometry(QX11Info::display(), client, &win, &x, &y, &w, &h, &bx, &by);
    clientGeometry = QRect(x,y,w,h);
}

void CWindow::getClientTitle()
{
    char *name;
    Atom type;
    int format;
    unsigned long nitems=0;
    unsigned long extra=0;
    unsigned char *data=NULL;
    Atom _net_wm_name = XInternAtom(QX11Info::display(), "_NET_WM_NAME", False);
    Atom wm_locale_name = XInternAtom(QX11Info::display(), "WM_LOCALE_NAME", False);

    XGetWindowProperty(QX11Info::display(), client, _net_wm_name, 0, 500, false, AnyPropertyType, &type, &format, &nitems, &extra, &data);

    if (data != NULL)
    {
        clientName = QString::fromUtf8((char *)data);
        XFree(data);
    }
    else if (XFetchName(QX11Info::display(), client, &name) && name != NULL)
    {
        clientName = name;
        XFree(name);
    }
    else  // use class hints
    {
        XClassHint ch;

        if (XGetClassHint(QX11Info::display(), client, &ch))
        {
            //res_name = ch.res_name;
            //res_class = ch.res_class;
            XFree(ch.res_name);
            XFree(ch.res_class);
            //wm_name = res_name;
        }
    }

    XGetWindowProperty(QX11Info::display(), client, wm_locale_name, 0, 500, false, AnyPropertyType, &type, &format, &nitems, &extra, &data);
    if (data != NULL)
    {
        XFree(data);
    }
}

