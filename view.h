#ifndef VIEW_H
#define VIEW_H

#include "headers.h"
#include "compositor.h"

class Compositor;
class View;

class View : public QWaylandView
{
    Q_OBJECT
public:

    View(Compositor *compositor); // Constructor

    Vertex vertices[256]; // Vertex list


    float zIndex = 0.01f; // Z-Position in OpenGL Coords
    unsigned radius = 10; // Size of radius

    unsigned int borderWidth = 1;
    unsigned int borderQ = 1; // Corner Radius Quiality ( Subdividitions )
    unsigned int topQuadCount, bottomQuadCount, cornerCount; // Amount of used vertices

    void calcVertexPos(); // Calculates all the vertex positions and colors of the view
    void calcTexturePos(); // Transform pixel coords into OpenGL texture coords (0 to 1)
    void toOpenGLPos(); // Transform pixel coords into OpenGL coords (-1 to 1)
    void setVertexCol(int index, QColor color); // Assign color to a vertex
    void setVertexPos(int index, float x, float y); // Assign position to a vertex
    void setTextureCord(int index, float x, float y); // Assign vertex texture coords

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

#endif
