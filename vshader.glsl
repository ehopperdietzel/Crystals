attribute vec4 pos;
attribute vec4 col;


varying vec4 finalColor;

attribute vec2 texCoordsIn;
varying vec2 texCoordsOut;

void main(void)
{
    gl_Position = pos;
    finalColor = col;
    texCoordsOut = texCoordsIn;
}
