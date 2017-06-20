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

    title->setAlignment(Qt::AlignCenter);
    title->setText("Terminal");

    setObjectName("TB");
    setStyleSheet("#TB{background:#FAFAFA;border-top-left-radius:8px;border-top-right-radius:8px;font-size:25px}");
}
