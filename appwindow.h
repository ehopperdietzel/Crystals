#ifndef APPWINDOW_H
#define APPWINDOW_H

#include "includes.h"
#include "topbarbutton.h"


class AppWindow : public QFrame
{
public:
    AppWindow(xcb_window_t _client);

    QBoxLayout *mainLayout = new QBoxLayout(QBoxLayout::TopToBottom,this);
    QFrame *container = new QFrame();

    QWidget *window = new QWidget();
    int client;

    QWidget *right = new QWidget(this);
    QFrame *topBar = new QFrame();
    QLabel *title = new QLabel();

    QBoxLayout *divider = new QBoxLayout(QBoxLayout::TopToBottom,container);
    QBoxLayout *topBarLayout = new QBoxLayout(QBoxLayout::LeftToRight,topBar);

    TopBarButton *cloBtn = new TopBarButton(":res/img/cloWin.png",":res/img/cloWin_A.png",QSize(12,12));
    TopBarButton *minBtn = new TopBarButton(":res/img/minWin.png",":res/img/minWin_A.png",QSize(14,14));
    TopBarButton *expBtn = new TopBarButton(":res/img/expWin.png",":res/img/expWin_A.png",QSize(14,14));

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
