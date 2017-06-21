#include "appwindowtitlebar.h"

AppWindowTitleBar::AppWindowTitleBar()
{
    topBarLayout->addWidget(cloBtn);
    topBarLayout->addWidget(minBtn);
    topBarLayout->addWidget(expBtn);
    topBarLayout->addWidget(title,1);
    topBarLayout->setContentsMargins(12,5,12,5);
    topBarLayout->addSpacing(80);
    topBarLayout->setSpacing(10);

    setFixedHeight(32);

    title->setAlignment(Qt::AlignCenter);
    title->setText("Application");
    title->setStyleSheet("font-size:10px;color:#333;font-weight:bold");

    setObjectName("TB");
    setStyleSheet("#TB{background: qlineargradient( x1:0 y1:0, x1:0 y2:1, stop:0 #FEFEFE, stop:0.4 #F9F9F9,stop:1 #F1F1F1);border-top-left-radius:8px;border-top-right-radius:8px;border-bottom:1px solid #CCC}");
}
