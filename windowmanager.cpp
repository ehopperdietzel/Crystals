#include "windowmanager.h"

Q_DECLARE_METATYPE(QList<int>)


WindowManager::WindowManager()
{
    qRegisterMetaTypeStreamOperators<QList<int> >("QList<int>");

    //Detecta si previamente se ha cambiado la resolucion de la pantalla
    if(settings.contains("RestoreWindows"))
    {
        QProcess::startDetached("firefox");
        restoreWindows();
    }
    else
    {
        //Inicia una terminal por defecto
        QProcess::startDetached("gnome-terminal");

    }
    connect(coreClient,SIGNAL(message(QString,QString)),this,SLOT(newCoreMessage(QString,QString)));
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

void WindowManager::newCoreMessage(QString name, QString data)
{
    if(name == "CHANGERESOLUTION")
    {
        settings.setValue("CurrentWindows",QVariant::fromValue(appWindows.keys()));
        coreClient->sendMessage("RELAUNCHCRYSTALS","NULL");
    }
}



void WindowManager::restoreWindows()
{
    QList<int> windowIDs = settings.value("CurrentWindows").value<QList<int> >();

    foreach (int wid, windowIDs)
        mapAppWindow(wid);

    settings.remove("CurrentWindows");

}
