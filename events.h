#ifndef EVENTS_H
#define EVENTS_H

#include "includes.h"


class Events : public QAbstractNativeEventFilter
{
public:
    Events();

    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);

};

#endif // EVENTS_H
