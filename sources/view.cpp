#include "headers/view.h"

View::View(Compositor *compositor)
    : compositor(compositor)
    , m_parentView(nullptr)
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

    for(float x = 0; x <= radius; x+=1.0f/cornerQuality)
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

    for(float x = 0; x <= radius; x+=1.0f/cornerQuality)
    {
        float y = qSqrt( qPow( radius - borderWidth , 2 ) - qPow( x , 2 ));
        setVertexCol(index,Qt::white);
        setVertexPos(index , x + (w - radius) , y + (h - radius) );
        index++;
    }


    // Left corner smooth border
    for(float x = 0; x <= radius; x+=1.0f/cornerQuality)
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
    for(float x = 0; x <= radius; x+=1.0f/cornerQuality)
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
        texture = buf.toOpenGLTexture();
    }

    return texture;
}


QSize View::size() const
{
    return surface()->size();
}

bool View::isCursor() const
{
    return surface() && surface()->isCursorSurface();
}






