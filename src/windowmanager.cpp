#include "windowmanager.h"


WindowManager::WindowManager()
{
    messages->move(10,10);
    messages->setFixedSize(200,50);
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
    if(windows.contains(wid))
        return;

    //Lo embedas en una ventana de Cuarzo
    CWindow *newWin = new CWindow(wid);
    windows.insert(wid,newWin);
}

void WindowManager::resizeAppWindow(int wid, int width, int height)
{
    //Detecta si ya esta mapeada
    if(!windows.contains(wid))
        return;

    //Le cambias el tamaño
    windows[wid]->resizeReq(width,height);
}

void WindowManager::destroyWindow(int wid)
{
   windows.take(wid)->closeWindow();
}

//Recibe los mensajes de SiO4
void WindowManager::newCoreMessage(QString name, QString data)
{

    //Cambio de resolucion
    if(name == "CHANGERESOLUTION")
    {

        QVariantMap savedClients;

        //Cambia de padres a todas los clientes para que no se cierren los procesos
        foreach (CWindow *win, windows) {

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

    //Vuelve a mapear los clientes
    foreach (QString key, wids.keys())
    {
        QVariantList clientData = wids[key].toList();

        messages->setText(QString::number(clientData[0].toInt()));
        /*
        CWindow *newWin = new CWindow(
                    clientData[0].toInt(),
                    clientData[1].toFloat(),
                    clientData[2].toFloat(),
                    clientData[3].toFloat(),
                    clientData[4].toFloat()
                );
                */
        CWindow *newWin = new CWindow(clientData[0].toInt());
        windows.insert(clientData[0].toInt(),newWin);

    }

    //Elimina el respaldo
    QFile::remove("/tmp/screens.json");

}



