#ifndef WINDOWCOMPOSITOR_H
#define WINDOWCOMPOSITOR_H

#include "headers.h"

QT_BEGIN_NAMESPACE

class QWaylandWlShell;
class QWaylandWlShellSurface;
class QWaylandXdgShellV5;
class QOpenGLTexture;
class Compositor;

class View : public QWaylandView
{
    Q_OBJECT
public:
    View(Compositor *compositor);
    QOpenGLTexture *getTexture();
    QOpenGLTextureBlitter::Origin textureOrigin() const;
    QPointF position() const { return m_position; }
    void setPosition(const QPointF &pos) { m_position = pos; }
    QSize size() const;
    bool isCursor() const;
    bool hasShell() const { return m_wlShellSurface; }
    void setParentView(View *parent) { m_parentView = parent; }
    View *parentView() const { return m_parentView; }
    QPointF parentPosition() const { return m_parentView ? (m_parentView->position() + m_parentView->parentPosition()) : QPointF(); }
    QSize windowSize() { return m_xdgSurface ? m_xdgSurface->windowGeometry().size() :  surface() ? surface()->size() : m_size; }
    QPoint offset() const { return m_offset; }

    qreal animationFactor() const {return m_animationFactor; }
    void setAnimationFactor(qreal f) {m_animationFactor = f; }

signals:
    void animationDone();

protected:
    void timerEvent(QTimerEvent *event);

private:
    friend class Compositor;
    Compositor *m_compositor;
    GLenum m_textureTarget;
    QOpenGLTexture *m_texture;
    QOpenGLTextureBlitter::Origin m_origin;
    QPointF m_position;
    QSize m_size;
    QWaylandWlShellSurface *m_wlShellSurface;
    QWaylandXdgSurfaceV5 *m_xdgSurface;
    QWaylandXdgPopupV5 *m_xdgPopup;
    View *m_parentView;
    QPoint m_offset;
    qreal m_animationFactor;
    QBasicTimer m_animationTimer;
    bool m_animationCountUp;

public slots:
    void onXdgSetMaximized();
    void onXdgUnsetMaximized();
    void onXdgSetFullscreen(QWaylandOutput *output);
    void onXdgUnsetFullscreen();
    void onOffsetForNextFrame(const QPoint &offset);

    void startAnimation(bool countUp);
    void cancelAnimation();
};

class Compositor : public QWaylandCompositor
{
    Q_OBJECT
public:
    Compositor(QWindow *window);
    ~Compositor();
    void create() override;

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
    QWindow *m_window;
    QList<View*> m_views;
    QWaylandWlShell *m_wlShell;
    QWaylandXdgShellV5 *m_xdgShell;
    QWaylandView m_cursorView;
    int m_cursorHotspotX;
    int m_cursorHotspotY;
};


QT_END_NAMESPACE

#endif // WINDOWCOMPOSITOR_H
