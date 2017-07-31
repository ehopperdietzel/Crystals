#ifndef WINDOWCOMPOSITOR_H
#define WINDOWCOMPOSITOR_H

#include "headers.h"
#include "view.h"

QT_BEGIN_NAMESPACE

class QWaylandWlShell;
class QWaylandWlShellSurface;
class QWaylandXdgShellV5;
class QOpenGLTexture;
class Compositor;
class View;


class Compositor : public QWaylandCompositor
{
    Q_OBJECT
public:
    Compositor(QWindow *window);
    ~Compositor();
    void create() override;

    QWindow *m_window;

    void startRender();
    void endRender();

    QList<View*> views() const { return m_views; }
    void raise(View *view);

    void handleMouseEvent(QWaylandView *target, QMouseEvent *me);
    void handleResize(View *target, const QSize &initialSize, const QPoint &delta, int edge);
    void handleDrag(View *target, QMouseEvent *me);

    QWaylandClient *popupClient() const;
    void closePopups();
protected:
    void adjustCursorSurface(QWaylandSurface *surface, int hotspotX, int hotspotY);

signals:
    void startMove();
    void startResize(int edge, bool anchored);
    void dragStarted(View *dragIcon);
    void frameOffset(const QPoint &offset);

public slots:
    void triggerRender();

private slots:
    void surfaceHasContentChanged();
    void surfaceDestroyed();
    void viewSurfaceDestroyed();
    void onStartMove();
    void onWlStartResize(QWaylandSeat *seat, QWaylandWlShellSurface::ResizeEdge edges);
    void onXdgStartResize(QWaylandSeat *seat, QWaylandXdgSurfaceV5::ResizeEdge edges);

    void startDrag();


    void onSurfaceCreated(QWaylandSurface *surface);
    void onWlShellSurfaceCreated(QWaylandWlShellSurface *wlShellSurface);
    void onXdgSurfaceCreated(QWaylandXdgSurfaceV5 *xdgSurface);
    void onXdgPopupRequested(QWaylandSurface *surface, QWaylandSurface *parent, QWaylandSeat *seat,
                             const QPoint &position, const QWaylandResource &resource);
    void onSetTransient(QWaylandSurface *parentSurface, const QPoint &relativeToParent, bool inactive);
    void onSetPopup(QWaylandSeat *seat, QWaylandSurface *parent, const QPoint &relativeToParent);

    void onSubsurfaceChanged(QWaylandSurface *child, QWaylandSurface *parent);
    void onSubsurfacePositionChanged(const QPoint &position);

    void updateCursor();
    void viewAnimationDone();
private:
    View *findView(const QWaylandSurface *s) const;
    QList<View*> m_views;
    QWaylandWlShell *m_wlShell;
    QWaylandXdgShellV5 *m_xdgShell;
    QWaylandView m_cursorView;
    int m_cursorHotspotX;
    int m_cursorHotspotY;
};


QT_END_NAMESPACE

#endif // WINDOWCOMPOSITOR_H
