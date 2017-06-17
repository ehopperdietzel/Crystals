#include "events.h"
#include "appwindow.h"
Events::Events()
{

}

bool Events::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
    if(eventType == "xcb_generic_event_t")
    {
        xcb_generic_event_t *event = static_cast<xcb_generic_event_t*>(message);

        if(event->response_type == XCB_MAP_REQUEST)
        {
            xcb_map_request_event_t *mapRequest = (xcb_map_request_event_t*)event;
            AppWindow *newWin = new AppWindow(mapRequest->window);
            newWin->show();
            //desktop->createWindow(mapRequest->window);
            return true;
        }

    }
    return false;
}
