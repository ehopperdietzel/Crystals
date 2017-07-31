// QT
#include <QGuiApplication>
#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QPointer>
#include <QtMath>

#include <QMouseEvent>
#include <QKeyEvent>
#include <QTouchEvent>

// Wayland
#include <QtWaylandCompositor/QWaylandCompositor>
#include <QtWaylandCompositor/QWaylandSurface>
#include <QtWaylandCompositor/QWaylandView>
#include <QtWaylandCompositor/QWaylandWlShellSurface>
#include <QtWaylandCompositor/QWaylandXdgSurfaceV5>
#include <QtWaylandCompositor/QWaylandXdgShellV5>
#include <QtWaylandCompositor/QWaylandWlShellSurface>
#include <QtWaylandCompositor/qwaylandseat.h>
#include <QtWaylandCompositor/qwaylanddrag.h>

// Open GL
#include <QOpenGLWindow>
#include <QOpenGLContext>
#include <QOpenGLTextureBlitter>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLWindow>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QVector2D>
#include <QVector3D>


#ifndef STRUCTURES
#define STRUCTURES

// Vertex structure
struct Vertex
{
    float position[3];
    float color[4];
    float texture[2];
};

// Background View Mode
enum ViewMode { Image, Color, Gradient };

// Background image size mode
enum ImageMode { ResizeToFill, KeepRatioToFill };

#endif
