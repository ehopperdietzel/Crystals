#include "appwindow.h"

AppWindow::AppWindow(xcb_window_t _client)
{
    //Almacena el WinID para usos futuros
    client = _client;

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
        window->setFixedSize(500,500);
    }
    else{
        window->setFixedSize(_w,_h);
    }
    move(_x,_y + 30);
    XMoveResizeWindow(QX11Info::display(),client,0,0,_w,_h);

    //Configura los estilos
    style();
}

void AppWindow::style()
{
    //Fondo transparente
    setAttribute(Qt::WA_TranslucentBackground);

    //Obtener eventos
    setMouseTracking(true);
    installEventFilter(this);

    window->installEventFilter(this);
    window->setMouseTracking(true);

    mainLayout->addWidget(topBar);
    mainLayout->addWidget(window,1);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);

    topBarLayout->addWidget(cloBtn);
    topBarLayout->addWidget(minBtn);
    topBarLayout->addWidget(expBtn);
    topBarLayout->addWidget(title,1);
    topBarLayout->setContentsMargins(12,5,12,5);
    topBarLayout->addSpacing(80);
    topBarLayout->setSpacing(10);

    title->setText("Terminal");
    title->setAlignment(Qt::AlignCenter);

    topBar->setFixedHeight(40);
    topBar->setObjectName("TB");
    topBar->setStyleSheet("#TB{background:#FAFAFA;border-top-left-radius:8px;border-top-right-radius:8px;font-size:15px}");
    topBar->installEventFilter(this);

    window->setObjectName("WD");
    window->setStyleSheet("#WD{background:#FAFAFA}");

    cloBtn->installEventFilter(this);

    show();
}

void AppWindow::resizeEvent(QResizeEvent *)
{
    //setWidgetBorderRadius(this,12);
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

bool AppWindow::eventFilter(QObject *watched, QEvent *event)
{

    if(event->type() == QEvent::HoverEnter)
    {
        //blabla
    }

    if(event->type() == QEvent::MouseButtonPress)
    {
        raise();
        if( (watched == topBar || watched == title) && !pressed)
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
        if(watched == cloBtn)
        {
            closeWindow();
        }
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

