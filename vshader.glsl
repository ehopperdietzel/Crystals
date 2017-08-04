attribute vec4 pos;
attribute vec4 col;
attribute vec2 texCoordsIn;

varying vec4 finalColor;
varying vec2 texCoordsOut;

uniform vec2 screenSize;
uniform vec2 viewSize;
uniform vec2 viewOffset;
uniform bool isBackground;

void main(void)
{
    // Set vertex color
    finalColor = col;

    if(isBackground)
    {
        // Set background position
        texCoordsOut = texCoordsIn;
        gl_Position = pos;
    }
    else
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

}
