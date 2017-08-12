#include "headers/window.h"
#include "headers/compositor.h"

Window::Window(Compositor *comp)
{
    // Save the compositor reference
    compositor = comp;

    // Resize event
    connect(compositor, &Compositor::startResize, this, &Window::startResize);

    // Drag event
    connect(compositor, &Compositor::dragStarted, this, &Window::startDrag);

    // Show window
    show();
}

void Window::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!program.link())
        close();

    // Bind shader pipeline for use
    if (!program.bind())
        close();

    // Select the current shader program
    glUseProgram(program.programId());

    // Get shader attributes locations
    posSlot = glGetAttribLocation(program.programId(), "pos");
    colSlot = glGetAttribLocation(program.programId(), "col");
    corSlot = glGetAttribLocation(program.programId(), "texCoordsIn");

    // Enable shader attributes
    glEnableVertexAttribArray(posSlot);
    glEnableVertexAttribArray(colSlot);
    glEnableVertexAttribArray(corSlot);

    // Get shader uniforms locations
    screenSizeUniform   = glGetUniformLocation(program.programId(), "screenSize");
    isBackgroundUniform = glGetUniformLocation(program.programId(), "isBackground");
    offsetUniform       = glGetUniformLocation(program.programId(), "viewOffset");
    viewSizeUniform     = glGetUniformLocation(program.programId(), "viewSize");

}


void Window::initializeGL()
{
    // Create a vertex buffer
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    // Create a vertex index buffer
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    // Sets the active texture location
    glActiveTexture(GL_TEXTURE0);

    // Send the triangles indices list for the background
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(background->Indices), background->Indices, GL_STATIC_DRAW);

    // Set default background image
    setBackground("/home/e/wallpaper.jpg");

    // Set default background color
    background->setColor(Qt::white);

    // Create shaders
    initShaders();

    // Set screen clear color
    glClearColor(1, 1, 1, 1); // White

    // Set the data index and size of each vertex
    glVertexAttribPointer(posSlot, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0); // Position
    glVertexAttribPointer(colSlot, 4, GL_FLOAT, GL_FALSE,sizeof(Vertex), (GLvoid*) (sizeof(float) * 3)); // Color
    glVertexAttribPointer(corSlot, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) (sizeof(float) * 7)); // Texture Cords
}

void Window::drawBackground()
{
    // Tells OpenGL it is the background
    glUniform1ui(isBackgroundUniform,true);

    // Selects the background texture
    glBindTexture(GL_TEXTURE_2D, background->texture->textureId());

    // Send the vertex list
    glBufferData(GL_ARRAY_BUFFER, sizeof(background->vertices), background->vertices, GL_STATIC_DRAW);

    //Draw Background
    glDrawElements(GL_TRIANGLES, sizeof(GLubyte)*6,GL_UNSIGNED_BYTE, 0);
}

void Window::drawView(View *view)
{

    // Calculates view vertices if size has changed
    if(view->previusSize != view->size())
    {
        // Saves the current view size
        view->previusSize = view->size();

        // Calculates the view vertices
        view->calcVertexPos();
    }


    // Gets background screenshot
    /*
    unsigned char pixels[4];
    glReadPixels(view->position().x(),view->position().y(),1,1,GL_RGB,GL_UNSIGNED_BYTE,&pixels);
    qDebug()<<pixels[0];
    */

    // Tells OpenGL it is not the background
    glUniform1ui(isBackgroundUniform,false);

    // Tells OpenGL the view position
    glUniform2f(offsetUniform,view->position().x(),view->position().y());

    // Tells OpenGL the view size
    glUniform2f(viewSizeUniform,view->size().width(),view->size().height());

    // Select current view texture
    glBindTexture(GL_TEXTURE_2D, view->getTexture()->textureId());

    // Sends the vertices list
    glBufferData(GL_ARRAY_BUFFER, sizeof(view->vertices), view->vertices, GL_STATIC_DRAW);

    // ---> Draw Surface

    // Draw Surface
    glDrawArrays(GL_TRIANGLE_FAN,0, view->surfaceCount);

    // Draw borders
    glDrawArrays(GL_TRIANGLE_STRIP,view->surfaceCount, view->borderCount);
}


void Window::setBackground(QString path)
{

    QOpenGLTexture *backgroundTexture = new QOpenGLTexture( QImage(path), QOpenGLTexture::DontGenerateMipMaps);
    backgroundTexture->setMinificationFilter(QOpenGLTexture::Linear);
    backgroundTexture->setMagnificationFilter(QOpenGLTexture::Linear);

    background->setImage(backgroundTexture);
    background->setMode(Image);
    background->setImageMode(KeepRatioToFill);

}



void Window::paintGL()
{

    // Begin rendering scene
    compositor->startRender();

    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable alpha blending
    glEnable(GL_BLEND);

    // Set blend mode
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Asign the screen size uniform
    glUniform2f(screenSizeUniform,width(),height());

    // Draw Background Image
    drawBackground();

    // Draw all views
    Q_FOREACH (View *view, compositor->views) {

        // Skip cursor
        if (view->isCursor())
            continue;

        // Skip if no texture
        auto texture = view->getTexture();
        if (!texture)
            continue;

        // Select current surface
        QWaylandSurface *surface = view->surface();

        // Check if surface is avaliable
        if ((surface && surface->hasContent()) || view->isBufferLocked()) {

            // Get surface size
            QSize s = view->size();

            // Skip if size is 0
            if (!s.isEmpty()) {

                // Draw view
                drawView(view);

            }
        }
    }

    // Finishes rendering
    compositor->endRender();
}

void Window::resizeGL(int, int)
{
    // Adjust background
    if(background->viewMode == Image)
        background->setImageMode(background->imageMode);
}

View *Window::viewAt(const QPointF &point)
{
    // Store the view
    View *ret = 0;

    // Loop all views
    Q_FOREACH (View *view, compositor->views)
    {
        // Skip if is the drag icon
        if (view == dragIconView)
            continue;

        // Check if point is in view rect
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
    initialSize = mouseView->size();
    grabState = ResizeGrab;
    resizeEdge = edge;
    resizeAnchored = anchored;
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
    switch (grabState)
    {
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
    }
        break;
    case DragGrab: {
        View *view = viewAt(e->localPos());
        compositor->handleDrag(view, e);
        if (dragIconView) {
            dragIconView->setPosition(e->localPos() + dragIconView->position());
            update();
        }
    }
        break;
    }
}

// Send mouse event to the compositor
void Window::sendMouseEvent(QMouseEvent *e, View *target)
{
    // Get the mouse coords
    QPointF mappedPos = e->localPos();

    // Check if view was found
    if (target) mappedPos -= target->position();

    // Create the mouse event
    QMouseEvent viewEvent(e->type(), mappedPos, e->localPos(), e->button(), e->buttons(), e->modifiers());

    // Send event to the compositor
    compositor->handleMouseEvent(target, &viewEvent);
}

// Send key press event to the compositor
void Window::keyPressEvent(QKeyEvent *e)
{
    compositor->defaultSeat()->sendKeyPressEvent(e->nativeScanCode());
}

// Send key release event to the compositor
void Window::keyReleaseEvent(QKeyEvent *e)
{
    compositor->defaultSeat()->sendKeyReleaseEvent(e->nativeScanCode());
}


