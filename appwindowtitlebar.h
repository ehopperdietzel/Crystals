#ifndef APPWINDOWTITLEBAR_H
#define APPWINDOWTITLEBAR_H

#include "includes.h"
#include "topbarbutton.h"

class AppWindowTitleBar : public QFrame
{
public:
    AppWindowTitleBar();
    QLabel *title = new QLabel();
    TopBarButton *cloBtn = new TopBarButton(":/img/window/cloWin.png",":/img/window/cloWin_A.png",QSize(10,10));
    TopBarButton *minBtn = new TopBarButton(":/img/window/minWin.png",":/img/window/minWin_A.png",QSize(10,10));
    TopBarButton *expBtn = new TopBarButton(":/img/window/expWin.png",":/img/window/expWin_A.png",QSize(10,10));
    QBoxLayout *topBarLayout = new QBoxLayout(QBoxLayout::LeftToRight,this);
};

#endif // APPWINDOWTITLEBAR_H
