#ifndef COMPOSITORWINDOW_H
#define COMPOSITORWINDOW_H

#include "headers.h"
#include "background.h"

QT_BEGIN_NAMESPACE

class Compositor;
class View;
class QOpenGLTexture;

class Window : public QOpenGLWindow
{
    Q_OBJECT
public:
    Window(Compositor *comp);
    void setBackground(QString path);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int, int) override;

    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;

    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;

    void initShaders();

private slots:
    void startMove();
    void startResize(int edge, bool anchored);
    void startDrag(View *dragIcon);

private:
    QOpenGLShaderProgram program;
    GLuint vertexBuffer, indexBuffer;

    // OpenGL Variables Location
    GLuint screenSizeUniform; // Screen Size ( pixels )
    GLuint offsetUniform; // View Position
    GLuint isBackgroundUniform; // Check if is drawing background
    GLuint viewSizeUniform; // View Size ( pixels )
    GLuint posSlot; // Vertex  pos
    GLuint colSlot; // Vertex  col
    GLuint corSlot; // Texture pos

    Background *background = new Background(this);

    enum GrabState { NoGrab, MoveGrab, ResizeGrab, DragGrab };

    View *viewAt(const QPointF &point);
    bool mouseGrab() const { return grabState != NoGrab ;}
    void drawBackground();
    void drawView(View *view);
    void sendMouseEvent(QMouseEvent *e, View *target);


    Compositor *compositor;
    QPointer<View> mouseView;
    GrabState grabState = NoGrab;
    QSize initialSize;
    int resizeEdge;
    bool resizeAnchored;
    QPointF resizeAnchorPosition, mouseOffset, initialMousePos;
    View *dragIconView;
};

QT_END_NAMESPACE

#endif // COMPOSITORWINDOW_H
