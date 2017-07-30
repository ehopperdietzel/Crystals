#include "background.h"



Background::Background(QOpenGLWindow *_window)
{
    window = _window;
}

void Background::setVertexPos(int index, float x, float y, float z)
{
    vertices[index].position[0] = x;
    vertices[index].position[1] = y;
    vertices[index].position[2] = z;
}

void Background::setTextureCord(int index, float x, float y)
{
    vertices[index].texture[0] = x;
    vertices[index].texture[1] = y;
}

void Background::setMode(ViewMode mode)
{
    viewMode = mode;

}

void Background::setColor(QColor color)
{
    for( int i = 0; i <= 4; i++)
    {
        vertices[i].color[0] = color.redF();
        vertices[i].color[1] = color.greenF();
        vertices[i].color[2] = color.blueF();
        vertices[i].color[3] = color.alphaF();
    }
}

void Background::setImage(QOpenGLTexture *_texture)
{
    texture = _texture;
}

void Background::setImageMode(ImageMode mode)
{
    imageMode = mode;
    screenSize = window->size();

    switch (imageMode) {

    case ResizeToFill:

        //Set Vertex Position
        setVertexPos(0,-1, 1);
        setVertexPos(1,-1,-1);
        setVertexPos(2, 1,-1);
        setVertexPos(3, 1, 1);


        //Set Texture Coords
        setTextureCord(0,0,0);
        setTextureCord(1,0,1);
        setTextureCord(2,1,1);
        setTextureCord(3,1,0);

        break;

    case KeepRatioToFill:

        //Set Vertex Position
        setVertexPos(0,-1, 1);
        setVertexPos(1,-1,-1);
        setVertexPos(2, 1,-1);
        setVertexPos(3, 1, 1);

        float relativeHeight = ((float)texture->height() / (float)texture->width()) * (float)screenSize.width();

        if(relativeHeight > window->height())
        {
            float clipped = ((float)relativeHeight - (float)screenSize.height()) / 2.0f;
            clipped = (1.0f / (float)screenSize.height()) * clipped;

            //Set Texture Coords
            setTextureCord(0,0,0 + clipped);
            setTextureCord(1,0,1 - clipped);
            setTextureCord(2,1,1 - clipped);
            setTextureCord(3,1,0 + clipped);


        }
        else
        {
            float texPropWidth = ((float)texture->width() / (float)texture->height()) * (float)screenSize.height();
            float clipped = ((float)texPropWidth - (float)screenSize.width()) / 2.0f;
            clipped = (1.0f / (float)screenSize.width()) * clipped;

            qDebug() << clipped;

            //Set Texture Coords
            setTextureCord(0,0 + clipped,0);
            setTextureCord(1,0 + clipped,1);
            setTextureCord(2,1 - clipped,1);
            setTextureCord(3,1 - clipped,0);

        }


        break;

    }
}


