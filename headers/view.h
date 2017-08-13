#ifndef VIEW_H
#define VIEW_H

#include "headers.h"
#include "compositor.h"
#include "socket.h"

class Compositor;
class View;

class View : public QWaylandView
{
    Q_OBJECT
public:

    View(Compositor *compositor); // Constructor

    // OpenGL variables
    Vertex vertices[256]; // Vertex list
    float zIndex = 0.01f; // Z-Position in OpenGL Coords
    float radius = 15.0f; // Size of radius
    uint borderWidth = 1; // Smooth border width
    uint surfaceCount, borderCount; // Amount of used vertices

    // Surface configuration
    uint role = WINDOW_MODE; // Surface role
    uint opacity = 255; // Surface Opacity
    QString title = ""; // Surface Title
    int surfaceId = -1; // Surface ID
    bool blur = false; // Surface Blur
    bool blurChanged = false;

    // OpenGL functions
    void calcVertexPos(); // Calculates all the vertex positions and colors of the view
    void setVertexCol(int index, QColor color); // Assign color to a vertex
    void setVertexPos(int index, float x, float y); // Assign position to a vertex
    void setTextureCord(int index, float x, float y); // Assign vertex texture coords

    QOpenGLTexture *getTexture();
    QPointF position() const { return m_position; }
    void setPosition(const QPointF &pos) { m_position = pos; }
    QSize size() const;
    bool isCursor() const;
    void setParentView(View *parent) { m_parentView = parent; }
    View *parentView() const { return m_parentView; }

    Compositor *compositor;
    QOpenGLTexture *texture;
    QPointF m_position, previusPosition;
    View *m_parentView;


    QSize previusSize; // For not recalc vertex positions


};

#endif
