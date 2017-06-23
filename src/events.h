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
    void getAtoms();
    void sendSupportedHints();

    Atom wm_protocols;
    Atom wm_delete_window;
    Atom wm_change_state;
    Atom wm_state;
    Atom wm_take_focus;
    Atom wm_resource_manager;
    Atom wm_colormaps;
    Atom wm_hints;
    Atom wm_normal_hints;
    Atom wm_name;
    Atom wm_transient_for;

    //Protocolo XDND
    Atom xdnd_aware;
    Atom xdnd_position;
    Atom xdnd_enter;
    Atom xdnd_finished;
    Atom xdnd_status;

    //Extensiones
    Atom _net_wm_name;
    Atom _net_wm_icon;
    Atom _net_supported;
    Atom _net_wm_user_time;
    Atom _net_wm_state;
    Atom _net_wm_window_type;
    Atom _net_wm_window_type_normal;
    Atom _net_wm_window_type_desktop;
    Atom _net_wm_window_type_dialog;
    Atom _net_wm_window_type_splash;
    Atom _net_wm_window_type_dnd;
    Atom _kde_net_wm_system_tray_window_for;
};

#endif // EVENTS_H
