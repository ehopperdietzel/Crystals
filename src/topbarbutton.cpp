#include "topbarbutton.h"

TopBarButton::TopBarButton(QString normalPix, QString activePix, QSize size)
{
    normal = QIcon(normalPix);
    active = QIcon(activePix);
    setIconSize(size);
    setFixedSize(size);
    setIcon(normal);
    setStyleSheet("border:none");
}

void TopBarButton::mousePressEvent(QMouseEvent *)
{
    setIcon(active);
}

void TopBarButton::mouseReleaseEvent(QMouseEvent *)
{
    setIcon(normal);
}
