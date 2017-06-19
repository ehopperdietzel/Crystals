#include "appwindow.h"

AppWindow::AppWindow(xcb_window_t _client)
{

    //Almacena el WinID para usos futuros

    client = _client;

    //Le dice al server que eventos quieres escuchar

    XSelectInput(QX11Info::display(),_client, KeyPressMask | KeyReleaseMask |  ButtonPressMask | ButtonReleaseMask | KeymapStateMask | ButtonMotionMask | PointerMotionMask |  EnterWindowMask | LeaveWindowMask |  FocusChangeMask |  VisibilityChangeMask |  ExposureMask | StructureNotifyMask | SubstructureRedirectMask | SubstructureNotifyMask );

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

    if(_w < 300 || _h < 300){
        QSize sSize = QApplication::desktop()->size();
        window->setFixedSize(sSize.width()/2,sSize.height()/2);
    }
    else{
        window->setFixedSize(_w,_h);
    }

    move(_x,_y + 30);



    //XMoveResizeWindow(QX11Info::display(),client,0,0,_w,_h);

    //Configura los estilos
    style();
}

void AppWindow::resizeReq(int w, int h)
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

void AppWindow::style()
{
    //Fondo transparente
    setAttribute(Qt::WA_TranslucentBackground);

    //Obtener eventos
    setMouseTracking(true);
    installEventFilter(this);
    titleBar->installEventFilter(this);

    //Ajusta el layout principal, con margin para que se vea el drop shadow
    mainLayout->addWidget(container);
    mainLayout->setMargin(20);

    //Ajusta el layout que separa la barra de titulo con la ventana
    containerLayout->addWidget(titleBar);
    containerLayout->addWidget(window,1);
    containerLayout->setMargin(0);
    containerLayout->setSpacing(0);

    //Añade fondo a la ventana visible
    container->setObjectName("WD");
    container->setStyleSheet("#WD{background:#FAFAFA;border-top-left-radius:8px;border-top-right-radius:8px;border:1px solid #AAA}");

    shadow->setBlurRadius(25);
    shadow->setColor(QColor(0,0,0,100));
    shadow->setOffset(5,5);
    container->setGraphicsEffect(shadow);

    show();
}

void AppWindow::resizeEvent(QResizeEvent *)
{
    QSize screenSize = QApplication::desktop()->size();
    perSize = QSize((float)100 / (float)screenSize.width() * (float)width()  , (float)100 / (float)screenSize.height() * (float)height());
    perPos = QPoint((float)100 / (float)screenSize.width() * (float)pos().x(), (float)100 / (float)screenSize.height() * (float)pos().y());
}

void AppWindow::setWidgetBorderRadius(QWidget *w,int radius) {

    QImage alpha(QSize(w->width(),w->height()),QImage::Format_ARGB32);
    alpha.fill(Qt::transparent);
    QPainter painter(&alpha);
    painter.setBrush(Qt::black);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawRoundedRect(0,0,w->width(),w->height(),0,8,Qt::AbsoluteSize);
    QPixmap mask = QPixmap::fromImage(alpha);
    w->setMask(mask.mask());

}

void AppWindow::closeWindow()
{
    delete this;
}

void AppWindow::changeResolution()
{
    QSize screenSize = QApplication::desktop()->size();
    QSize newSize = QSize((float)screenSize.width() / (float)100  * (float)perSize.width()  , (float)screenSize.height() / (float)100  * (float)perSize.height() );
    QPoint newPos = QPoint((float)screenSize.width() / (float)100  * (float)perPos.x()       , (float)screenSize.height() / (float)100  * (float)perPos.y());
    resize(newSize);
    move(newPos);
}

bool AppWindow::eventFilter(QObject *watched, QEvent *event)
{

    if(event->type() == QEvent::HoverEnter)
    {
        //blabla
    }

    if(event->type() == QEvent::MouseButtonPress)
    {
        raise();

        if( (watched == titleBar ) && !pressed)
        {
            QMouseEvent *me = static_cast<QMouseEvent*>(event);
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
            QPoint newPos = QCursor::pos() - prevCur + prevWin;
            move(newPos);
            return true;
        }
    }

    return false;
}

