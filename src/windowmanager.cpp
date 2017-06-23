#include "windowmanager.h"


WindowManager::WindowManager()
{
    messages->move(10,10);
    messages->show();

    //Detecta si previamente se ha cambiado la resolucion de la pantalla
    if(QFileInfo::exists("/tmp/screens.json"))
    {
        restoreWindows();
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

//Recibe los mensajes de SiO4
void WindowManager::newCoreMessage(QString name, QString data)
{

    //Cambio de resolucion
    if(name == "CHANGERESOLUTION")
    {

        QVariantMap savedClients;

        //Cambia de padres a todas los clientes para que no se cierren los procesos
        foreach (AppWindow *win, appWindows) {

            //Guarda la informacion: data << Wid << X << Y << W << H
            QVariantList clientData;
            clientData.append(win->client);
            clientData.append(win->window->pos().x());
            clientData.append(win->window->pos().y());
            clientData.append(win->window->width());
            clientData.append(win->window->height());

            savedClients.insert(QString::number(win->client),clientData);
            win->hide();
            XReparentWindow(QX11Info::display(),win->client,QX11Info::appRootWindow(),500,500);
            win->show();
            win->hide();

        }

        //Guarda los wid de los clientes actuales
        QFile saver("/tmp/screens.json");
        saver.open(QIODevice::ReadWrite);
        QJsonDocument doc = QJsonDocument::fromVariant(savedClients);
        saver.write(doc.toBinaryData());
        saver.close();

        //Le dice a SiO4 que reinicie Crystals
        coreClient->sendMessage("RELAUNCHCRYSTALS","NULL");

    }
}


//Vuelve a mapear los clientes, luego de cambio de resolucion

void WindowManager::restoreWindows()
{

    //Lee los Wids guardados
    QFile saver("/tmp/screens.json");
    saver.open(QIODevice::ReadWrite);
    QJsonDocument doc = QJsonDocument::fromBinaryData(saver.readAll());
    QVariantMap wids = doc.toVariant().toMap();
    saver.close();



    foreach (QString key, wids.keys())
    {
        QVariantList clientData = wids[key].toList();

        messages->setText(QString::number(clientData[0].toInt()));
        AppWindow *newWin = new AppWindow(
                    clientData[0].toInt(),
                    clientData[1].toFloat(),
                    clientData[2].toFloat(),
                    clientData[3].toFloat(),
                    clientData[4].toFloat()
                );
        appWindows.insert(clientData[0].toInt(),newWin);

    }

    QFile::remove("/tmp/screens.json");

}



