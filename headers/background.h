#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "headers.h"

class Background : public QObject
{
    Q_OBJECT
public:
    const GLubyte Indices[6] = { 0, 1, 2, 2, 3, 0 };
    Background(QOpenGLWindow *window);


    QOpenGLTexture *texture;
    QOpenGLWindow *window;

    ViewMode viewMode;
    ImageMode imageMode;
    Vertex vertices[4];
    QSize screenSize;

    void setMode( ViewMode mode );
    void setColor( QColor color );
    void setImage(QOpenGLTexture *texture );
    void setImageMode( ImageMode mode );

private:
    void setVertexPos(int index, float x, float y, float z = -1);
    void setTextureCord(int index, float x, float y);





};

#endif // BACKGROUND_H
