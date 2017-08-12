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

    QColor surfaceColor = QColor(255,255,255,opacity);

    /* -------------- Surface -------------*/

    // Top Left
    setVertexPos(0,borderWidth,0);
    setVertexCol(0, surfaceColor);

    // Top Right
    setVertexPos(1,w - borderWidth,0);
    setVertexCol(1, surfaceColor);

    uint index = 2;

    // Right corner
    for(float x = radius - borderWidth; x >= 0.0f ; x -= (radius - borderWidth)/radius)
    {
        float y = qSqrt( qPow( radius - borderWidth , 2 ) - qPow( x  , 2 ));
        setVertexCol(index,surfaceColor);
        setVertexPos(index , w - radius + x , h - radius + y );
        index++;

    }

    // Left corner
    for(float x = 0.0f; x <= radius - borderWidth ; x += (radius - borderWidth)/radius)
    {
        float y = qSqrt( qPow( radius - borderWidth , 2 ) - qPow( x  , 2 ));
        setVertexCol(index,surfaceColor);
        setVertexPos(index , radius - x , h - radius + y );
        index++;

    }

    // Save surface vertex count
    surfaceCount = index;



    /* -------------- Border -------------*/


    // Top Right less border
    setVertexPos(index,w,0);
    setVertexCol(index,Qt::transparent);
    index++;

    // Top Right
    setVertexPos(index,w - borderWidth,0);
    setVertexCol(index,surfaceColor);
    index++;

    int X = radius;

    // Right corner
    for(float x = radius - borderWidth; x >= 0.0f ; x -= (radius - borderWidth)/radius)
    {
        // Out
        float y = qSqrt( qPow( radius , 2 ) - qPow( X  , 2 ));
        setVertexCol(index,Qt::transparent);
        setVertexPos(index , w - radius + X , h - radius + y );
        index++;
        X--;

        // In
        y = qSqrt( qPow( radius - borderWidth , 2 ) - qPow( x  , 2 ));
        setVertexCol(index,surfaceColor);
        setVertexPos(index , w - radius + x , h - radius + y );
        index++;

    }

    X = 0;
    // Left corner
    for(float x = 0.0f; x <= radius - borderWidth ; x += (radius - borderWidth)/radius)
    {
        // Out
        float y = qSqrt( qPow( radius , 2 ) - qPow( X  , 2 ));
        setVertexCol(index,Qt::transparent);
        setVertexPos(index , radius - X , h - radius + y );
        index++;
        X++;

        // In
        y = qSqrt( qPow( radius - borderWidth , 2 ) - qPow( x  , 2 ));
        setVertexCol(index,surfaceColor);
        setVertexPos(index , radius - x , h - radius + y );
        index++;

    }

    // Top Right
    setVertexPos(index,0,0);
    setVertexCol(index,Qt::transparent);
    index++;

    // Top Left less border
    setVertexPos(index,borderWidth,0);
    setVertexCol(index,surfaceColor);
    index++;



    borderCount = index - surfaceCount;







    /*

    // Set all vertex white
    for(int i = 0; i<14; i++)
        setVertexCol(i,surfaceColor);

    // Top Square ( Triangle Strip 0-7 )
    setVertexPos(0,0,0); // Top Left For border
    setVertexPos(1,0,h - radius); // Bottom left for border

    setVertexPos(2,borderWidth,0); // Top Left
    setVertexPos(3,borderWidth,h - radius); // Bottom left
    setVertexPos(4,w - borderWidth,0); // Top Right
    setVertexPos(5,w - borderWidth,h - radius); // Bottom Right

    setVertexPos(6,w,0); // Top Right for border
    setVertexPos(7,w,h-radius); // Bottom Right for border

    topQuadCount = 8;


    // Bottom Border ( Triangle Strip 8-11 )

    setVertexPos(8,w - radius,h - borderWidth); // Top right
    setVertexPos(9,radius,h - borderWidth); // Top left
    setVertexPos(10,w - radius,h); // Bottom right for border
    setVertexPos(11,radius,h - borderWidth); // Bottom left for border

    bottomQuadCount = 4;

    // Set transparent vertices
    setVertexCol(0,Qt::transparent);
    setVertexCol(1,Qt::transparent);
    setVertexCol(6,Qt::transparent);
    setVertexCol(7,Qt::transparent);

    setVertexCol(10,Qt::transparent);
    setVertexCol(11,Qt::transparent);

    // Left Corner Center
    setVertexPos(12,radius,h-radius);
    setVertexCol(12, surfaceColor);

    int index = 13;
    cornerCount = 0;

    // Left corner
    for(float x = 0; x <= radius; x+=1.0f/cornerQuality)
    {
        float y = qSqrt( qPow( radius - borderWidth , 2 ) - qPow( x - radius , 2 ));
        setVertexPos( index, x , y + (h - radius) );
        setVertexCol( index, surfaceColor);
        index++;
        cornerCount++;
    }

    // Right Corner Center
    setVertexPos(index,w - radius,h - radius);
    index++;

    for(float x = 0; x <= radius; x+=1.0f/cornerQuality)
    {
        float y = qSqrt( qPow( radius - borderWidth , 2 ) - qPow( x , 2 ));
        setVertexCol(index,surfaceColor);
        setVertexPos(index , x + (w - radius) , y + (h - radius) );
        index++;
    }


    // Left corner smooth border
    for(float x = 0; x <= radius; x+=1.0f/cornerQuality)
    {
        float y = qSqrt( qPow( radius - borderWidth , 2 ) - qPow( x - radius , 2 ));
        setVertexPos( index, x , y + (h - radius) );
        setVertexCol( index, surfaceColor);
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
        setVertexCol(index,surfaceColor);
        setVertexPos(index , x + (w - radius) , y + (h - radius) );
        index++;

        y = qSqrt( qPow( radius, 2 ) - qPow( x , 2 ));
        setVertexCol(index,Qt::transparent);
        setVertexPos(index , x + (w - radius) , y + (h - radius) );
        index++;
    }
    */
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






