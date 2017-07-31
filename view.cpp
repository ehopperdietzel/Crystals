#include "view.h"

View::View(Compositor *compositor)
    : m_compositor(compositor)
    , m_textureTarget(GL_TEXTURE_2D)
    , m_texture(0)
    , m_wlShellSurface(nullptr)
    , m_xdgSurface(nullptr)
    , m_xdgPopup(nullptr)
    , m_parentView(nullptr)
    , m_animationFactor(1.0)
{
    for(int i = 0; i < 4 ; i++)
    {
        setVertexCol(i,Qt::red);
    }

}

void View::calcVertexPos()
{
    float h = size().height();
    float w = size().width();

    setVertexPos(0,0,0);
    setVertexPos(1,0,h);
    setVertexPos(2,w,h);
    setVertexPos(3,w,0);

    setTextureCord(0,0,0);
    setTextureCord(1,0,1);
    setTextureCord(2,1,1);
    setTextureCord(3,1,0);

    /*
    setVertexPos(0,0,0);
    setVertexPos(1,0, ((float)h/4.0f));
    setVertexPos(2,0, (float)(h/4.0f)*2.0f);
    setVertexPos(3,0, ((float)h/4.0f)*3.0f);

    for(int i = 0; i <= 12 ; i++)
    {
        float x = (radius/12.0f)*i;
        float y = -qSqrt(radius - qPow(x - radius ,2)) - h + radius;
        setVertexPos(i+4, x, y);
    }

    for(int i = 0; i <= 12 ; i++)
    {
        float x = w - radius + (12.0f/radius)*i;
        float y = -qSqrt(radius - qPow(x,2)) - h + radius;
        setVertexPos(i+17, x, y);
    }

    setVertexPos(23,w,0);
    setVertexPos(22,w, ((float)h/4.0f));
    setVertexPos(21,w, ((float)h/4.0f)*2.0f);
    setVertexPos(20,w, ((float)h/4.0f)*3.0f);
    */
}

void View::toOpenGLPos()
{
    float h = m_compositor->m_window->height();
    float w = m_compositor->m_window->width();
    float x = position().x();
    float y = position().y();

    for(int i = 0; i < 4 ; i++)
    {
        setVertexPos(
             i,
             ( (2.0f / w) * (vertices[i].position[0] + x) ) - 1.0f,
             1.0f - ( (2.0f / h) * (vertices[i].position[1] + y) )
        );
    }
}

void View::setVertexCol(int index, QColor color)
{
    vertices[index].color[0] = color.redF();
    vertices[index].color[1] = color.greenF();
    vertices[index].color[2] = color.blueF();
    vertices[index].color[3] = color.alphaF();
}

void View::setVertexPos(int index, float x, float y)
{
    vertices[index].position[0] = x;
    vertices[index].position[1] = y;
    vertices[index].position[2] = zIndex;
}

void View::setTextureCord(int index, float x, float y)
{
    vertices[index].texture[0] = x;
    vertices[index].texture[1] = y;
}

QOpenGLTexture *View::getTexture()
{
    if (advance()) {
        QWaylandBufferRef buf = currentBuffer();
        m_texture = buf.toOpenGLTexture();
        if (surface()) {
            m_size = surface()->size();
            m_origin = buf.origin() == QWaylandSurface::OriginTopLeft
                    ? QOpenGLTextureBlitter::OriginTopLeft
                    : QOpenGLTextureBlitter::OriginBottomLeft;
        }
    }

    return m_texture;
}

QOpenGLTextureBlitter::Origin View::textureOrigin() const
{
    return m_origin;
}

QSize View::size() const
{
    return surface() ? surface()->size() : m_size;
}

bool View::isCursor() const
{
    return surface() && surface()->isCursorSurface();
}


void View::onXdgSetMaximized()
{
    m_xdgSurface->sendMaximized(output()->geometry().size());

    // An improvement here, would have been to wait for the commit after the ack_configure for the
    // request above before moving the window. This would have prevented the window from being
    // moved until the contents of the window had actually updated. This improvement is left as an
    // exercise for the reader.
    setPosition(QPoint(0, 0));
}

void View::onXdgUnsetMaximized()
{
    m_xdgSurface->sendUnmaximized();
}

void View::onXdgSetFullscreen(QWaylandOutput* clientPreferredOutput)
{
    QWaylandOutput *outputToFullscreen = clientPreferredOutput
            ? clientPreferredOutput
            : output();

    m_xdgSurface->sendFullscreen(outputToFullscreen->geometry().size());

    // An improvement here, would have been to wait for the commit after the ack_configure for the
    // request above before moving the window. This would have prevented the window from being
    // moved until the contents of the window had actually updated. This improvement is left as an
    // exercise for the reader.
    setPosition(outputToFullscreen->position());
}

void View::onOffsetForNextFrame(const QPoint &offset)
{
    m_offset = offset;
    setPosition(position() + offset);
}


void View::timerEvent(QTimerEvent *event)
{
    if (event->timerId() != m_animationTimer.timerId())
        return;

    m_compositor->triggerRender();

    if (m_animationCountUp) {
        m_animationFactor += .08;
        if (m_animationFactor > 1.0) {
            m_animationFactor = 1.0;
            m_animationTimer.stop();
            emit animationDone();
        }
    } else {
        m_animationFactor -= .08;
        if (m_animationFactor < 0.01) {
            m_animationFactor = 0.01;
            m_animationTimer.stop();
            emit animationDone();
        }
    }
}

void View::startAnimation(bool countUp)
{
    m_animationCountUp = countUp;
    m_animationFactor = countUp ? .1 : 1.0;
    m_animationTimer.start(20, this);
}

void View::cancelAnimation()
{
    m_animationFactor = 1.0;
    m_animationTimer.stop();
}

void View::onXdgUnsetFullscreen()
{
    onXdgUnsetMaximized();
}
