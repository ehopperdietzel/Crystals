#ifndef TOPBARBUTTON_H
#define TOPBARBUTTON_H

#include "includes.h"

class TopBarButton : public QPushButton
{
    Q_OBJECT
public:
    QIcon normal,active;
    TopBarButton(QString normalPix,QString activePix,QSize size);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
};

#endif // TOPBARBUTTON_H
