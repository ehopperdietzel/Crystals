// QT
#include <QGuiApplication>
#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QPointer>
#include <QtMath>

#include <QLocalServer>
#include <QLocalSocket>

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

#ifndef SURFACE_MODES
#define SURFACE_MODES
    #define WINDOW_MODE 0
    #define FRAMELESS_MODE 1
    #define TOPBAR_MODE 2
    #define FILEMANAGER_MODE 3
    #define DOCK_MODE 4
#endif

#ifndef MESSAGES
#define MESSAGES


/* ------------------ In Messages Types ------------------ */

// Register App
#define REGISTER_APP 0
typedef struct{
    unsigned int type; // Message type
    unsigned int pid; // App proccess Id
}RegisterAppStruct;

// Configure Surface
#define SURFACE_CONFIG 1
typedef struct{
    unsigned int type = SURFACE_CONFIG; // Message type
    unsigned int id; // Surface Id
    unsigned int role; // Surface Role
    unsigned int x; // X pos
    unsigned int y; // Y pos
    unsigned int opacity; // Surface Opacity
    char title[128]; // Surface Title
}SurfaceConfigStruct;

// Send Surface Role
#define SURFACE_ROLE 2
typedef struct{
    unsigned int type = SURFACE_ROLE; // Message type
    unsigned int id; // Surface Id
    unsigned int role; // Surface Role
}SurfaceRoleStruct;

// Send Surface Position
#define SURFACE_POS 3
typedef struct{
    unsigned int type = SURFACE_POS; // Message type
    unsigned int id; // Surface Id
    int x; // X cords
    int y; // Y cords
}SurfacePosStruct;

// Send Surface Title
#define SURFACE_TITLE 4
typedef struct{
    unsigned int type = SURFACE_TITLE; // Message type
    unsigned int id; // Surface Id
    char title[128]; // Surface Title
}SurfaceTitleStruct;

// Send Surface Opacity
#define SURFACE_OPACITY 5
typedef struct{
    unsigned int type = SURFACE_OPACITY; // Message type
    unsigned int id; // Surface Id
    unsigned int opacity; // Surface Opacity
}SurfaceOpacityStruct;

// Send Surface blur Request
#define SURFACE_BLUR 6
typedef struct{
    unsigned int type = SURFACE_BLUR; // Message type
    unsigned int id; // Surface Id
    bool activate; // Turn ON/OFF blur
}SurfaceBlurStruct;

// Send Surface Minimize
#define SURFACE_MINIMIZE 7
typedef struct{
    unsigned int type = SURFACE_MINIMIZE; // Message type
    unsigned int id; // Surface Id
    bool minimize; // Turn ON/OFF minimize
}SurfaceMinimizeStruct;

// Send Surface Expand
#define SURFACE_EXPAND 8
typedef struct{
    unsigned int type = SURFACE_EXPAND; // Message type
    unsigned int id; // Surface Id
    bool expand; // Turn ON/OFF expand
    unsigned int expandMode;
}SurfaceExpandStruct;



/* ------------------ Out Messages Types ------------------ */

// Messages Out

// App Registered Event
#define REGISTERED_APP 0
typedef struct{
    unsigned int type = REGISTERED_APP; // Message type
}RegisteredAppStruct;

// Surface Id Registered
#define REGISTERED_SURFACE 1
typedef struct{
    unsigned int type = REGISTERED_SURFACE; // Message type
    unsigned int id; // Message type
}RegisteredSurfaceStruct;

// Surface Blur Image
#define SURFACE_BLUR_IMAGE 2
typedef struct{
    unsigned int type = SURFACE_BLUR_IMAGE; // Message type
    unsigned int id; // Surface desitnation
    unsigned int image[4*4][3]; // 8x8 image
}SurfaceBlurImageStruct;

#endif

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

// IPC messages
enum MessageType{ Caca };

// View type
enum ViewType{ Paradiso, TitleBar, Roller };


#endif
