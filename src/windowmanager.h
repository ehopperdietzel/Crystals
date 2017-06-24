#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "includes.h"
#include "cwindow.h"
#include "coreclient.h"

class WindowManager : public QObject
{
    Q_OBJECT
public:
    explicit WindowManager();

    //Client - Window
    QMap<int,CWindow*>windows;

    Huincha huincha;
    QSettings settings;
    CoreClient *coreClient = new CoreClient();
    QLabel *messages = new QLabel("Holiwis");

signals:

public slots:
    void newCoreMessage(QString name, QString data);
    void restoreWindows();

    //Clients Events
    void mapAppWindow(int wid);
    void resizeAppWindow(int wid, int width, int height);
    void destroyWindow(int wid);
};

#endif // WINDOWMANAGER_H
