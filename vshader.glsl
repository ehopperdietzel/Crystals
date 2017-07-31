attribute vec4 pos;
attribute vec4 col;

varying vec4 finalColor;
varying vec2 texCoordsOut;

attribute vec2 texCoordsIn;


void main(void)
{
    gl_Position = pos;
    finalColor = col;
    texCoordsOut = texCoordsIn;
}
