#ifndef WINDOWCOMPOSITOR_H
#define WINDOWCOMPOSITOR_H

#include "headers.h"
#include "window.h"
#include "view.h"
#include "socket.h"

QT_BEGIN_NAMESPACE

// Incoming message types

class QOpenGLTexture;
class Compositor;
class View;
class Socket;


class Compositor : public QWaylandCompositor
{
    Q_OBJECT
public:
    Compositor();
    QLocalServer *server = new QLocalServer(this); // IPC for views
    Window *window = new Window(this);
    QWaylandOutput *output = new QWaylandOutput(this, window);
    QWaylandWlShell *wlShell = new QWaylandWlShell(this);
    QList<View*> views;
    QList<Socket*> sockets;
    QWaylandView cursor;
    int m_cursorHotspotX;
    int m_cursorHotspotY;

    void startRender();
    void endRender();
    void raise(View *view);
    void setScreenResolution(QSize size);

    void handleMouseEvent(QWaylandView *target, QMouseEvent *me);
    void handleResize(View *target, const QSize &initialSize, const QPoint &delta, int edge);
    void handleDrag(View *target, QMouseEvent *me);

    View *findViewById(int id);
    View *findViewByIdAndPid(int id,int pid);
    Socket *findSocketByPId(int id);

protected:
    void adjustCursorSurface(QWaylandSurface *surface, int hotspotX, int hotspotY);

signals:
    void startResize(int edge, bool anchored);
    void dragStarted(View *dragIcon);

public slots:
    void triggerRender();
    void newClientConnected();
    void newClientMessage();

private slots:
    void socketDisconnected();
    void surfaceHasContentChanged();
    void surfaceDestroyed();
    void viewSurfaceDestroyed();

    void startDrag();

    void titleChanged();


    void onSurfaceCreated(QWaylandSurface *surface);
    void onWlShellCreated(QWaylandWlShellSurface *wlShellSurface);


    void onSubsurfaceChanged(QWaylandSurface *child, QWaylandSurface *parent);
    void onSubsurfacePositionChanged(const QPoint &position);

    void updateCursor();
private:
    View *findView(const QWaylandSurface *s) const;
};


QT_END_NAMESPACE

#endif // WINDOWCOMPOSITOR_H
