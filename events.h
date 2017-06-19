#ifndef EVENTS_H
#define EVENTS_H

#include "includes.h"
#include "windowmanager.h"

//Esta clase se utiliza para obtener los eventos de X11
//Debe ser instalada en la aplicacion ( Como se muestra en main.cpp )

class Events : public QAbstractNativeEventFilter
{

public:
    Events();
    WindowManager *windowManager = new WindowManager;
    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);


};

#endif // EVENTS_H
