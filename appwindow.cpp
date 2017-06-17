#include "appwindow.h"
#include <QRegion>
#include <QGraphicsDropShadowEffect>

AppWindow::AppWindow(xcb_window_t _client)
{
    client = _client;

    QWindow *win = QWindow::fromWinId(client);
    window = QWidget::createWindowContainer(win);
    window->setFixedSize(500, 500);

    style(); //SETUP VIEW
}

void AppWindow::style()
{
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    setAttribute(Qt::WA_TranslucentBackground);
    shadow->setBlurRadius(40);
    shadow->setColor(Qt::black);
    setGraphicsEffect(shadow);
    setPalette(Qt::transparent);
    setStyleSheet("AppWindow{border-radius:50}");
    setAutoFillBackground(false);
    setMouseTracking(true);
    installEventFilter(this);
    window->installEventFilter(this);
    window->setMouseTracking(true);

    mainLayout->addWidget(container);
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

    topBar->setFixedHeight(30);
    topBar->setObjectName("TB");
    topBar->setStyleSheet("#TB{background:#FAFAFA;border-top-left-radius:12px;border-top-right-radius:12px}");
    topBar->installEventFilter(this);

    window->setObjectName("WD");
    window->setStyleSheet("#WD{background:#FAFAFA;border-radius:12px}");


    container->setObjectName("CT");
    container->setStyleSheet("#CT{background:#FAFAFA;border-radius:12px}");

    divider->setMargin(0);
    divider->setSpacing(0);
    divider->addWidget(topBar);
    divider->addWidget(window,1);

    //right->setFixedWidth(100);
    //right->setAttribute(Qt::WA_Hover);
    //right->show();
    //right->installEventFilter(this);

    cloBtn->installEventFilter(this);
    show();
}

void AppWindow::resizeEvent(QResizeEvent *)
{

    setWidgetBorderRadius(container,12);
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
        if( watched == right)
        {
            setCursor(Qt::SizeHorCursor);
        }
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

