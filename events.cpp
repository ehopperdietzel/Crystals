#include "events.h"

//Constructor

Events::Events(){}

//Eventos de X11

bool Events::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(result); // Para detener las alertas de NO uso.
    //Si es un evento de X11
    if(eventType == "xcb_generic_event_t")
    {
        //Parsea el evento
        xcb_generic_event_t *event = static_cast<xcb_generic_event_t*>(message);

        //Map request ( Una aplicacion quiere ser mostrada )
        if(event->response_type == XCB_MAP_REQUEST)
        {
            //Parseas el evento especifico
            xcb_map_request_event_t *mapRequest = (xcb_map_request_event_t*)event;

            windowManager->mapAppWindow(mapRequest->window);

            return true;
        }

        //Resize request ( Una aplicacion quiere cambiar de tamaño )
        if(event->response_type == XCB_RESIZE_REQUEST)
        {
            //Parseas el evento especifico
            xcb_resize_request_event_t *resizeRequest = (xcb_resize_request_event_t*)event;

            windowManager->resizeAppWindow(resizeRequest->window,resizeRequest->width,resizeRequest->height);

            return true;
        }

    }
    return false;
}


