#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "includes.h"
#include "appwindow.h"

class WindowManager : public QObject
{
    Q_OBJECT
public:
    explicit WindowManager();
    QMap<int,AppWindow*>appWindows;

signals:

public slots:
    void mapAppWindow(int wid);
    void resizeAppWindow(int wid, int width, int height);
};

#endif // WINDOWMANAGER_H
