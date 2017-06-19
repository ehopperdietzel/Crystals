#include "events.h"
#include "appwindow.h"

//Constructor

Events::Events(){}

//Eventos de X11

bool Events::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
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

            //Lo embedas en una ventana de Cuarzo
            AppWindow *newWin = new AppWindow(mapRequest->window);

            return true;
        }

    }
    return false;
}
