#include "window.h"

#include <QMouseEvent>
#include <QOpenGLWindow>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QMatrix4x4>

#include "compositor.h"
#include <QtWaylandCompositor/qwaylandseat.h>

#include <QVector2D>
#include <QVector3D>

struct Vertex
{
    float position[3];
    float color[4];
    float texture[2];
};

Vertex vertices[] = {
    {{ 1,-1, 0}, {1, 0, 0, 1}, {1,1}},
    {{ 1, 1, 0}, {0, 1, 0, 1}, {1,0}},
    {{-1, 1, 0}, {0, 0, 1, 1}, {0,0}},
    {{-1,-1, 0}, {0, 0, 0, 1}, {0,1}}
};

GLubyte Indices[] = {
     0, 1, 2,
     2, 3, 0
};

Window::Window()
    : backgroundTexture(0)
    , compositor(0)
    , grabState(NoGrab)
    , dragIconView(0)
{
}

void Window::setCompositor(Compositor *comp) {
    compositor = comp;
    connect(compositor, &Compositor::startMove, this, &Window::startMove);
    connect(compositor, &Compositor::startResize, this, &Window::startResize);
    connect(compositor, &Compositor::dragStarted, this, &Window::startDrag);
}


void Window::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/glsl/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/glsl/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!program.link())
        close();

    // Bind shader pipeline for use
    if (!program.bind())
        close();

    glUseProgram(program.programId());

    posSlot = glGetAttribLocation(program.programId(), "pos");
    colSlot = glGetAttribLocation(program.programId(), "col");
    corSlot = glGetAttribLocation(program.programId(), "texCoordsIn");
    texSlot = glGetUniformLocation(program.programId(), "Texture");

    glEnableVertexAttribArray(posSlot);
    glEnableVertexAttribArray(colSlot);
    glEnableVertexAttribArray(corSlot);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, backgroundTexture->textureId());
    glUniform1i(texSlot, 0);


}


void Window::initializeGL()
{


    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);


    //Set background image
    setBackground("/home/e/wallpaper.jpg");

    //Create shaders
    initShaders();

    //Set screen clear color
    glClearColor(1, 1, 1, 1);

}

void Window::drawBackground()
{

    glVertexAttribPointer(posSlot, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(colSlot, 4, GL_FLOAT, GL_FALSE,sizeof(Vertex), (GLvoid*) (sizeof(float) * 3));
    glVertexAttribPointer(corSlot, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) (sizeof(float) * 7));

    glDrawElements(GL_TRIANGLES, sizeof(Indices)/sizeof(Indices[0]),GL_UNSIGNED_BYTE, 0);
}


void Window::setBackground(QString path)
{
    backgroundTexture = new QOpenGLTexture( QImage(path), QOpenGLTexture::DontGenerateMipMaps);

    backgroundTexture->setMinificationFilter(QOpenGLTexture::Linear);

    backgroundTexture->setMagnificationFilter(QOpenGLTexture::Linear);

}


QPointF Window::getAnchorPosition(const QPointF &position, int resizeEdge, const QSize &windowSize)
{
    float y = position.y();
    if (resizeEdge & QWaylandXdgSurfaceV5::ResizeEdge::TopEdge)
        y += windowSize.height();

    float x = position.x();
    if (resizeEdge & QWaylandXdgSurfaceV5::ResizeEdge::LeftEdge)
        x += windowSize.width();

    return QPointF(x, y);
}

QPointF Window::getAnchoredPosition(const QPointF &anchorPosition, int resizeEdge, const QSize &windowSize)
{
    return anchorPosition - getAnchorPosition(QPointF(), resizeEdge, windowSize);
}

void Window::paintGL()
{

    //Begin rendering scene
    compositor->startRender();

    //Clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    //Loop all views
    Q_FOREACH (View *view, compositor->views()) {

        //Skip cursor
        if (view->isCursor())
            continue;

        //Skip if no texture
        auto texture = view->getTexture();
        if (!texture)
            continue;

        //Select current surface
        QWaylandSurface *surface = view->surface();

        //Check if surface is avaliable
        if ((surface && surface->hasContent()) || view->isBufferLocked()) {

            //Get surface size
            QSize s = view->size();

            //Skip if size is 0
            if (!s.isEmpty()) {

                if (mouseView == view && grabState == ResizeGrab && resizeAnchored)
                    view->setPosition(getAnchoredPosition(resizeAnchorPosition, resizeEdge, s));
                QPointF pos = view->position() + view->parentPosition();
                QRectF surfaceGeometry(pos, s);
                auto surfaceOrigin = view->textureOrigin();
                auto sf = view->animationFactor();
                QRectF targetRect(surfaceGeometry.topLeft() * sf, surfaceGeometry.size() * sf);
                backgroundTexture = view->getTexture();
            }
        }
    }


    //Draw Background Image
    drawBackground();

    compositor->endRender();
}

View *Window::viewAt(const QPointF &point)
{
    View *ret = 0;
    Q_FOREACH (View *view, compositor->views()) {
        if (view == dragIconView)
            continue;
        QRectF geom(view->position(), view->size());
        if (geom.contains(point))
            ret = view;
    }
    return ret;
}

void Window::startMove()
{
    grabState = MoveGrab;
}

void Window::startResize(int edge, bool anchored)
{
    initialSize = mouseView->windowSize();
    grabState = ResizeGrab;
    resizeEdge = edge;
    resizeAnchored = anchored;
    resizeAnchorPosition = getAnchorPosition(mouseView->position(), edge, mouseView->surface()->size());
}

void Window::startDrag(View *dragIcon)
{
    grabState = DragGrab;
    dragIconView = dragIcon;
    compositor->raise(dragIcon);
}

void Window::mousePressEvent(QMouseEvent *e)
{
    if (mouseGrab())
        return;
    if (mouseView.isNull()) {
        mouseView = viewAt(e->localPos());
        if (!mouseView) {
            compositor->closePopups();
            return;
        }
        if (e->modifiers() == Qt::AltModifier || e->modifiers() == Qt::MetaModifier)
            grabState = MoveGrab; //start move
        else
            compositor->raise(mouseView);
        initialMousePos = e->localPos();
        mouseOffset = e->localPos() - mouseView->position();

        QMouseEvent moveEvent(QEvent::MouseMove, e->localPos(), e->globalPos(), Qt::NoButton, Qt::NoButton, e->modifiers());
        sendMouseEvent(&moveEvent, mouseView);
    }
    sendMouseEvent(e, mouseView);
}

void Window::mouseReleaseEvent(QMouseEvent *e)
{
    if (!mouseGrab())
        sendMouseEvent(e, mouseView);
    if (e->buttons() == Qt::NoButton) {
        if (grabState == DragGrab) {
            View *view = viewAt(e->localPos());
            compositor->handleDrag(view, e);
        }
        mouseView = 0;
        grabState = NoGrab;
    }
}

void Window::mouseMoveEvent(QMouseEvent *e)
{
    switch (grabState) {
    case NoGrab: {
        View *view = mouseView ? mouseView.data() : viewAt(e->localPos());
        sendMouseEvent(e, view);
        if (!view)
            setCursor(Qt::ArrowCursor);
    }
        break;
    case MoveGrab: {
        mouseView->setPosition(e->localPos() - mouseOffset);
        update();
    }
        break;
    case ResizeGrab: {
        QPoint delta = (e->localPos() - initialMousePos).toPoint();
        compositor->handleResize(mouseView, initialSize, delta, resizeEdge);
    }
        break;
    case DragGrab: {
        View *view = viewAt(e->localPos());
        compositor->handleDrag(view, e);
        if (dragIconView) {
            dragIconView->setPosition(e->localPos() + dragIconView->offset());
            update();
        }
    }
        break;
    }
}

void Window::sendMouseEvent(QMouseEvent *e, View *target)
{
    QPointF mappedPos = e->localPos();
    if (target)
        mappedPos -= target->position();
    QMouseEvent viewEvent(e->type(), mappedPos, e->localPos(), e->button(), e->buttons(), e->modifiers());
    compositor->handleMouseEvent(target, &viewEvent);
}

void Window::keyPressEvent(QKeyEvent *e)
{
    compositor->defaultSeat()->sendKeyPressEvent(e->nativeScanCode());
}

void Window::keyReleaseEvent(QKeyEvent *e)
{
    compositor->defaultSeat()->sendKeyReleaseEvent(e->nativeScanCode());
}


