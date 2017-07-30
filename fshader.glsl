
varying lowp vec4 finalColor;
varying lowp vec2 texCoordsOut;

uniform bool onlyColor;
uniform sampler2D Texture;

void main(void)
{
    if(onlyColor)
       gl_FragColor = finalColor;
    else
       gl_FragColor = texture2D(Texture, texCoordsOut);
}
