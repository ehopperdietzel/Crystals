attribute vec4 pos;
attribute vec4 col;
attribute vec2 texCoordsIn;

varying vec4 finalColor;
varying vec2 texCoordsOut;

uniform vec2 screenSize;
uniform vec2 viewSize;
uniform vec2 viewOffset;
uniform lowp int Mode;
uniform bool inverted;

uniform highp vec2 textureSize;

varying highp vec2 blurTextureCoords[11];

void main(void)
{
    // Set vertex color
    finalColor = col;


    // Set background position
    if(Mode == 1)
    {

        texCoordsOut = texCoordsIn;
        if(inverted)
        {
            texCoordsOut.y = 1.0f - texCoordsIn.y;
        }

        gl_Position = pos;
    }
    // Set view position
    if(Mode == 0)
    {
        // Set View Position
        gl_Position.x = (2.0f/screenSize.x)*(viewOffset.x + pos.x) - 1.0f;
        gl_Position.y = 1.0f - (2.0f/screenSize.y)*(viewOffset.y + pos.y);
        gl_Position.z = pos.z;
        gl_Position.w = 1.0f;

        // Set Texture Vertex Position
        texCoordsOut.x = (pos.x)/viewSize.x;
        texCoordsOut.y = (pos.y)/viewSize.y;
    }
    // Calc horizontal blur
    if(Mode == 2)
    {
        float pixelSize = 1.0f/screenSize.x;

        for(int i=-5;i<=5;i++)
        {
            blurTextureCoords[i+5] = texCoordsIn + vec2(pixelSize*float(i),0.0f);
        }
    }

    // Calc vertical blur
    if(Mode == 3)
    {
        float pixelSize = 1.0f/viewSize.y;

        for(int i=-5;i<=5;i++)
        {
            blurTextureCoords[i+5] = texCoordsIn + vec2(0.0f,pixelSize*float(i));
        }
    }

}
