#ifndef APPWINDOWTITLEBAR_H
#define APPWINDOWTITLEBAR_H

#include "includes.h"
#include "windowbuttons.h"

class TopBar : public QFrame
{
public:
    TopBar()
    {
        topBarLayout->addWidget(buttons);
        topBarLayout->addWidget(title,1);
        topBarLayout->setContentsMargins(10,5,10,6);
        topBarLayout->addSpacing(80);
        topBarLayout->setSpacing(10);

        setFixedHeight(30);

        title->setAlignment(Qt::AlignCenter);
        title->setText("Application");
        title->setStyleSheet("font-size:11px;color:#222;font-family:Lucida Sans");

        setObjectName("TB");
        setStyleSheet("#TB{background: qlineargradient( x1:0 y1:0, x1:0 y2:1, stop:0 #FFF, stop:0.4 #FEFEFE,stop:1 #FDFDFD);border-top-left-radius:8px;border-top-right-radius:8px;border-bottom:1px solid #CCC}");
    }

    QLabel *title = new QLabel();
    WindowButtons *buttons = new WindowButtons();
    QHBoxLayout *topBarLayout = new QHBoxLayout(this);
};

#endif // APPWINDOWTITLEBAR_H

