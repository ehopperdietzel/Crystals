varying highp vec4 finalColor;
varying highp vec2 texCoordsOut;

uniform bool onlyColor;
uniform sampler2D Texture;


void main(void)
{
       gl_FragColor = texture2D(Texture, texCoordsOut) * finalColor;
}
