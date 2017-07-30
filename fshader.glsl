varying lowp vec4 finalColor;

varying lowp vec2 texCoordsOut;
uniform sampler2D Texture;

void main(void)
{
    gl_FragColor = texture2D(Texture, texCoordsOut);
}
