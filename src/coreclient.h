#ifndef CORECLIENT_H
#define CORECLIENT_H

#include "includes.h"

class CoreClient : public QLocalSocket
{
    Q_OBJECT
public:
    CoreClient()
    {
        connectToServer("CuarzoServer");
        connect(this,SIGNAL(readyRead()),this,SLOT(newMessage()));
    }

public slots:
    void newMessage()
    {
        QString fullMessage = QString::fromUtf8(readAll());
        QString key = fullMessage.mid(0,16);
        if(key != "CUARZOSOFTWARE16")
            return;


        QString name = fullMessage.mid(16,32).replace("%","");
        QString data = fullMessage.mid(48,128).replace("%","");

        message(name,data);

    }
    void sendMessage(QString name, QString data)
    {
        // ESTRUCTURA DEL MENSAJE ( NO USAR % )
        // 16 bytes -> Llave Secreta
        // 32 bytes -> Nombre del Mensaje
        // 128 bytes -> Informacion

        while(name.length()<32)
            name.append("%");

        while(data.length()<128)
            data.append("%");

        QString message = "CUARZOSOFTWARE16" + name + data;
        write(message.toUtf8());
    }


signals:
    void message(QString name,QString data);
};

#endif // CORECLIENT_H
