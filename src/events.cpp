#include "events.h"

//Constructor

Events::Events()
{
    //Obtener permisos y extensiones para el WM
    getAtoms();
    sendSupportedHints();

}

//Eventos de X11

bool Events::nativeEventFilter(const QByteArray &eventType, void *message, long *)
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

            windowManager->mapAppWindow(mapRequest->window);

            return true;
        }

        //Cuando un cliente cambia de padre
        if(event->response_type == XCB_REPARENT_NOTIFY)
        {
            //Parseas el evento especifico
            //xcb_reparent_notify_event_t *reparentEvent = (xcb_reparent_notify_event_t*)event;

            return true;
        }

        //Move Request ( Una aplicacion quiere moverse )
        if(event->response_type == XCB_CHANGE_WINDOW_ATTRIBUTES)
        {
            //Parseas el evento especifico
            xcb_change_window_attributes_request_t *moveRequest = (xcb_change_window_attributes_request_t*)event;

            //windowManager->resizeAppWindow(resizeRequest->window,resizeRequest->width,resizeRequest->height);

            return true;
        }

        //Resize request ( Una aplicacion quiere cambiar de tamaño )
        if(event->response_type == XCB_RESIZE_REQUEST)
        {
            //Parseas el evento especifico
            xcb_resize_request_event_t *resizeRequest = (xcb_resize_request_event_t*)event;

            windowManager->messages->setText(QString::number(resizeRequest->width));

            windowManager->resizeAppWindow(resizeRequest->window,resizeRequest->width,resizeRequest->height);

            return true;
        }


        //Resize request ( Una aplicacion quiere cambiar de tamaño )
        if(event->response_type == XCB_DESTROY_NOTIFY)
        {
            //Parseas el evento especifico
            xcb_destroy_notify_event_t *destroyRequest = (xcb_destroy_notify_event_t*)event;

            windowManager->destroyWindow(destroyRequest->window);

            return true;
        }

    }
    return false;
}

//Obtiene los atomos

void Events::getAtoms()
{
    //Protocolos requeridos por ICCCM
    wm_protocols = XInternAtom(QX11Info::display(), "WM_PROTOCOLS", False);
    wm_delete_window = XInternAtom(QX11Info::display(), "WM_DELETE_WINDOW", False);
    wm_change_state = XInternAtom(QX11Info::display(), "WM_CHANGE_STATE", False);
    wm_state = XInternAtom(QX11Info::display(), "WM_STATE", False);
    wm_take_focus = XInternAtom(QX11Info::display(), "WM_TAKE_FOCUS", False);
    wm_resource_manager = XInternAtom(QX11Info::display(), "RESOURCE_MANAGER", False);
    wm_colormaps = XInternAtom(QX11Info::display(), "WM_COLORMAP_WINDOWS", False);
    wm_hints = XInternAtom(QX11Info::display(), "WM_HINTS", False);
    wm_normal_hints = XInternAtom(QX11Info::display(), "WM_NORMAL_HINTS", False);
    wm_name = XInternAtom(QX11Info::display(), "WM_NAME", False);
    wm_transient_for = XInternAtom(QX11Info::display(), "WM_TRANSIENT_FOR", False);
    xdnd_aware = XInternAtom(QX11Info::display(), "XdndAware", False);
    xdnd_position = XInternAtom(QX11Info::display(), "XdndPosition", False);
    xdnd_enter = XInternAtom(QX11Info::display(), "XdndEnter", False);
    xdnd_finished = XInternAtom(QX11Info::display(), "XdndFinished", False);
    xdnd_status = XInternAtom(QX11Info::display(), "XdndStatus", False);

    //Extensiones para EWMH
    _net_wm_name = XInternAtom(QX11Info::display(), "_NET_WM_NAME", False);
    _net_wm_icon = XInternAtom(QX11Info::display(), "_NET_WM_ICON", False);
    _net_wm_user_time = XInternAtom(QX11Info::display(), "_NET_WM_USER_TIME", False);
    _net_wm_state = XInternAtom(QX11Info::display(), "_NET_WM_STATE", False);
    _net_supported = XInternAtom(QX11Info::display(), "_NET_SUPPORTED", False);
    _net_wm_window_type = XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE", False);
    _net_wm_window_type_normal = XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE_NORMAL", False);
    _net_wm_window_type_desktop = XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE_DESKTOP", False);
    _net_wm_window_type_dialog = XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE_DIALOG", False);
    _net_wm_window_type_splash = XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE_SPLASH", False);
    _net_wm_window_type_dnd = XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE_DND", False);
    _kde_net_wm_system_tray_window_for = XInternAtom(QX11Info::display(), "_KDE_NET_WM_SYSTEM_TRAY_WINDOW_FOR", False);
}

//Un maximo de 5 valores de 32 bits por mensaje

void Events::sendSupportedHints()
{
    Atom _net_supported = XInternAtom(QX11Info::display(), "_NET_SUPPORTED", False);
    XClientMessageEvent xev1;
    XClientMessageEvent xev2;
    XClientMessageEvent xev3;
    XClientMessageEvent xev4;
    XClientMessageEvent xev5;
    XClientMessageEvent xev6;

    xev1.type = ClientMessage;
    xev1.window = QApplication::desktop()->winId();
    xev1.message_type = _net_supported;
    xev1.format = 32;
    xev1.data.l[0] = wm_protocols;
    xev1.data.l[1] = wm_delete_window;
    xev1.data.l[2] = wm_change_state;
    xev1.data.l[3] = wm_state;
    xev1.data.l[4] = wm_take_focus;
    XSendEvent(QX11Info::display(), QApplication::desktop()->winId(), False,
               (SubstructureNotifyMask | SubstructureRedirectMask), (XEvent *)&xev1);

    xev2.type = ClientMessage;
    xev2.window = QApplication::desktop()->winId();
    xev2.message_type = _net_supported;
    xev2.format = 32;
    xev2.data.l[0] = wm_resource_manager;
    xev2.data.l[1] = wm_colormaps;
    xev2.data.l[2] = wm_hints;
    xev2.data.l[3] = wm_normal_hints;
    xev2.data.l[4] = wm_name;
    XSendEvent(QX11Info::display(), QApplication::desktop()->winId(), False,
               (SubstructureNotifyMask | SubstructureRedirectMask), (XEvent *)&xev2);

    xev3.type = ClientMessage;
    xev3.window = QApplication::desktop()->winId();
    xev3.message_type = _net_supported;
    xev3.format = 32;
    xev3.data.l[0] = wm_transient_for;
    xev3.data.l[1] = _net_wm_name;
    xev3.data.l[2] = _net_wm_icon;
    xev3.data.l[3] = _net_supported;
    xev3.data.l[4] = _net_wm_user_time;
    XSendEvent(QX11Info::display(), QApplication::desktop()->winId(), False,
               (SubstructureNotifyMask | SubstructureRedirectMask), (XEvent *)&xev3);

    xev4.type = ClientMessage;
    xev4.window = QApplication::desktop()->winId();
    xev4.message_type = _net_supported;
    xev4.format = 32;
    xev4.data.l[0] = _net_wm_window_type;
    xev4.data.l[1] = _net_wm_window_type_normal;
    xev4.data.l[2] = _net_wm_window_type_desktop;
    xev4.data.l[3] = _net_wm_window_type_dialog;
    xev4.data.l[4] = _net_wm_window_type_splash;
    XSendEvent(QX11Info::display(), QApplication::desktop()->winId(), False,
               (SubstructureNotifyMask | SubstructureRedirectMask), (XEvent *)&xev4);

    xev5.type = ClientMessage;
    xev5.window = QApplication::desktop()->winId();
    xev5.message_type = _net_supported;
    xev5.format = 32;
    xev5.data.l[0] = _kde_net_wm_system_tray_window_for;
    xev5.data.l[1] = _net_wm_state;
    xev5.data.l[2] = _net_wm_window_type_dnd;
    xev5.data.l[3] = xdnd_aware;
    xev5.data.l[4] = xdnd_position;
    XSendEvent(QX11Info::display(), QApplication::desktop()->winId(), False,
               (SubstructureNotifyMask | SubstructureRedirectMask), (XEvent *)&xev5);

    xev6.type = ClientMessage;
    xev6.window = QApplication::desktop()->winId();
    xev6.message_type = _net_supported;
    xev6.format = 32;
    xev6.data.l[0] = xdnd_enter;
    xev6.data.l[1] = xdnd_finished;
    xev6.data.l[2] = xdnd_status;
    XSendEvent(QX11Info::display(), QApplication::desktop()->winId(), False,
               (SubstructureNotifyMask | SubstructureRedirectMask), (XEvent *)&xev6);
}

