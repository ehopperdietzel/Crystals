#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "includes.h"
#include "appwindow.h"
#include "coreclient.h"

class WindowManager : public QObject
{
    Q_OBJECT
public:
    explicit WindowManager();

    Huincha huincha;
    QMap<int,AppWindow*>appWindows;
    QSettings settings;
    CoreClient *coreClient = new CoreClient();
    QLabel *messages = new QLabel("Holiwis");

signals:

public slots:
    void mapAppWindow(int wid);
    void resizeAppWindow(int wid, int width, int height);
    void newCoreMessage(QString name, QString data);
    void restoreWindows();
};

#endif // WINDOWMANAGER_H
