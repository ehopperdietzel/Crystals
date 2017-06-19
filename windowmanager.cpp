#include "windowmanager.h"

WindowManager::WindowManager()
{
    connect(QApplication::desktop(),SIGNAL(resized(int)),this,SLOT(resolutionChanged()));
}

void WindowManager::resolutionChanged()
{
    foreach (AppWindow *win, appWindows)
    {
        win->changeResolution();
    }
}

void WindowManager::mapAppWindow(int wid)
{
    //Detecta si ya esta mapeada
    if(appWindows.contains(wid))
        return;

    //Lo embedas en una ventana de Cuarzo
    AppWindow *newWin = new AppWindow(wid);
    appWindows.insert(wid,newWin);
}

void WindowManager::resizeAppWindow(int wid, int width, int height)
{
    //Detecta si ya esta mapeada
    if(!appWindows.contains(wid))
        return;

    //Le cambias el tamaño
    appWindows[wid]->resizeReq(width,height);
}
