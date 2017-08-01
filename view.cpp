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
    for(int i = 0; i < 2*radius + 8 ; i++)
    {
        setVertexCol(i,Qt::yellow);
    }

}

void View::calcVertexPos()
{
    float h = size().height();
    float w = size().width();

    setVertexPos(0,w/2,h/2); // Center vertex
    setVertexPos(1,0,0); // Top Left
    setVertexPos(2,0,h - radius); // Top Left

    // Bottom Left border radius
    for(int x = 0; x <= radius; x++)
    {
        float y = qSqrt( qPow( radius , 2 ) - qPow( x - radius , 2 ));

        setVertexPos( x + 3 , x , y + (h - radius) );
    }

    // Bottom Right border radius
    for(int x = 0; x <= radius; x++)
    {
        float y = qSqrt( qPow( radius , 2 ) - qPow( x , 2 ));

        setVertexPos( x + 4 + radius , x + (w - radius) , y + (h - radius) );
    }

    setVertexPos(2*radius + 5,w,0); // Top Right
    setVertexPos(2*radius + 6,0,0); // Top Left (Again)

    calcTexturePos();

}

void View::calcTexturePos()
{
    float x = 1.0f / (float)size().width();
    float y = 1.0f / (float)size().height();

    for(int i = 0; i < 2*radius+9; i++)
    {
        setTextureCord(i,vertices[i].position[0]/size().width(), vertices[i].position[1]/size().height());
    }
}

void View::toOpenGLPos()
{
    float h = m_compositor->m_window->height();
    float w = m_compositor->m_window->width();
    float x = position().x();
    float y = position().y();

    for(int i = 0; i < 2*radius+8 ; i++)
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
