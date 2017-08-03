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




}

void View::calcVertexPos()
{
    float h = size().height(); // View Height
    float w = size().width(); // View Width

    // Set all vertex white
    for(int i = 0; i<14; i++)
        setVertexCol(i,Qt::white);

    // Top Square ( Triangle Strip 0-7 )
    setVertexPos(0,0,0);
    setVertexPos(1,0,h - radius);
    setVertexPos(2,borderWidth,0);
    setVertexPos(3,borderWidth,h - radius);
    setVertexPos(4,w - borderWidth,0);
    setVertexPos(5,w - borderWidth,h - radius);
    setVertexPos(6,w,0);
    setVertexPos(7,w,h-radius);

    topQuadCount = 8;

    // Bottom Square ( Triangle Strip 8-13 )
    setVertexPos(8,radius,h-radius); // Left Corner Center
    setVertexPos(9,w-radius,h-radius); // Right Corner Center
    setVertexPos(10,radius,h-borderWidth);
    setVertexPos(11,w-radius,h-borderWidth);
    setVertexPos(12,radius,h);
    setVertexPos(13,w-radius,h);

    bottomQuadCount = 6;

    // Set transparent vertices
    setVertexCol(0,Qt::transparent);
    setVertexCol(1,Qt::transparent);
    setVertexCol(6,Qt::transparent);
    setVertexCol(7,Qt::transparent);

    setVertexCol(12,Qt::transparent);
    setVertexCol(13,Qt::transparent);


    // Bottom Left border radius
    setVertexPos(14,radius,h-radius); // Left Corner Center
    setVertexCol(14, Qt::white);

    int index = 15;
    cornerCount = 0;

    for(float x = 0; x <= radius; x+=1.0f/borderQ)
    {
        float y = qSqrt( qPow( radius - borderWidth , 2 ) - qPow( x - radius , 2 ));
        setVertexPos( index, x , y + (h - radius) );
        setVertexCol( index, Qt::white);
        index++;
        cornerCount++;
    }

    // Bottom Right border radius
    setVertexPos(index,radius,h-radius); // Right Corner Center
    index++;

    for(float x = 0; x <= radius; x+=1.0f/borderQ)
    {
        float y = qSqrt( qPow( radius - borderWidth , 2 ) - qPow( x , 2 ));
        setVertexCol(index,Qt::white);
        setVertexPos(index , x + (w - radius) , y + (h - radius) );
        index++;
    }


    // Left corner smooth border
    for(float x = 0; x <= radius; x+=1.0f/borderQ)
    {
        float y = qSqrt( qPow( radius - borderWidth , 2 ) - qPow( x - radius , 2 ));
        setVertexPos( index, x , y + (h - radius) );
        setVertexCol( index, Qt::white);
        index++;

        y = qSqrt( qPow( radius, 2 ) - qPow( x - radius , 2 ));
        setVertexPos( index, x , y + (h - radius) );
        setVertexCol( index, Qt::transparent);
        index++;
    }

    // Right corner smooth border
    for(float x = 0; x <= radius; x+=1.0f/borderQ)
    {
        float y = qSqrt( qPow( radius - borderWidth , 2 ) - qPow( x , 2 ));
        setVertexCol(index,Qt::white);
        setVertexPos(index , x + (w - radius) , y + (h - radius) );
        index++;

        y = qSqrt( qPow( radius, 2 ) - qPow( x , 2 ));
        setVertexCol(index,Qt::transparent);
        setVertexPos(index , x + (w - radius) , y + (h - radius) );
        index++;
    }



    calcTexturePos();
    toOpenGLPos();

}

void View::calcTexturePos()
{
    float x = 1.0f / (float)size().width();
    float y = 1.0f / (float)size().height();

    for(int i = 0; i < topQuadCount+bottomQuadCount+2+6*cornerCount; i++)
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

    for(int i = 0; i < topQuadCount+bottomQuadCount+2+6*cornerCount ; i++)
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