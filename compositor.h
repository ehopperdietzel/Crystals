#ifndef WINDOWCOMPOSITOR_H
#define WINDOWCOMPOSITOR_H

#include "headers.h"
#include "window.h"
#include "view.h"

QT_BEGIN_NAMESPACE


class QOpenGLTexture;
class Compositor;
class View;


class Compositor : public QWaylandCompositor
{
    Q_OBJECT
public:
    Compositor();
    Window *window = new Window(this);
    QWaylandOutput *output = new QWaylandOutput(this, window);
    QList<View*> views;
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

protected:
    void adjustCursorSurface(QWaylandSurface *surface, int hotspotX, int hotspotY);

signals:
    void startResize(int edge, bool anchored);
    void dragStarted(View *dragIcon);

public slots:
    void triggerRender();

private slots:
    void surfaceHasContentChanged();
    void surfaceDestroyed();
    void viewSurfaceDestroyed();

    void startDrag();


    void onSurfaceCreated(QWaylandSurface *surface);


    void onSubsurfaceChanged(QWaylandSurface *child, QWaylandSurface *parent);
    void onSubsurfacePositionChanged(const QPoint &position);

    void updateCursor();
private:
    View *findView(const QWaylandSurface *s) const;
};


QT_END_NAMESPACE

#endif // WINDOWCOMPOSITOR_H
