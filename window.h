
#ifndef COMPOSITORWINDOW_H
#define COMPOSITORWINDOW_H

#include <QOpenGLWindow>
#include <QPointer>
#include <QOpenGLTextureBlitter>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

QT_BEGIN_NAMESPACE

class Compositor;
class View;
class QOpenGLTexture;

class Window : public QOpenGLWindow
{
public:
    Window();

    void setCompositor(Compositor *comp);
    void setBackground(QString path);

protected:
    void initializeGL() override;
    void paintGL() override;

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
    QOpenGLTexture *backgroundTexture;
    GLuint vertexBuffer, indexBuffer;

    GLuint posSlot; //Vertex  pos
    GLuint colSlot; //Vertex  col
    GLuint corSlot; //Texture pos
    GLuint texSlot; //Texture uni


    enum GrabState { NoGrab, MoveGrab, ResizeGrab, DragGrab };

    View *viewAt(const QPointF &point);
    bool mouseGrab() const { return grabState != NoGrab ;}
    void drawBackground();
    void sendMouseEvent(QMouseEvent *e, View *target);
    static QPointF getAnchoredPosition(const QPointF &anchorPosition, int resizeEdge, const QSize &windowSize);
    static QPointF getAnchorPosition(const QPointF &position, int resizeEdge, const QSize &windowSize);

    Compositor *compositor;
    QPointer<View> mouseView;
    GrabState grabState;
    QSize initialSize;
    int resizeEdge;
    bool resizeAnchored;
    QPointF resizeAnchorPosition, mouseOffset, initialMousePos;
    View *dragIconView;
};

QT_END_NAMESPACE

#endif // COMPOSITORWINDOW_H
