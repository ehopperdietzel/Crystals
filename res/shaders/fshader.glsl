uniform highp vec2 textureSize;
uniform sampler2D Texture;
uniform lowp int Mode;

precision mediump float;
precision mediump int;

varying mediump vec4 finalColor;
varying mediump vec2 texCoordsOut;
varying highp vec2 blurTextureCoords[11];

void main(void){

    if(Mode == 2)
    {
        vec4 sum = vec4(0.0);
        sum += texture2D(Texture, blurTextureCoords[0]) * 0.0093f;
        sum += texture2D(Texture, blurTextureCoords[1]) * 0.028002f;
        sum += texture2D(Texture, blurTextureCoords[2]) * 0.065984f;
        sum += texture2D(Texture, blurTextureCoords[3]) * 0.121703f;
        sum += texture2D(Texture, blurTextureCoords[4]) * 0.175713f;
        sum += texture2D(Texture, blurTextureCoords[5]) * 0.198596f;
        sum += texture2D(Texture, blurTextureCoords[6]) * 0.175713f;
        sum += texture2D(Texture, blurTextureCoords[7]) * 0.121703f;
        sum += texture2D(Texture, blurTextureCoords[8]) * 0.065984f;
        sum += texture2D(Texture, blurTextureCoords[9]) * 0.028002f;
        sum += texture2D(Texture, blurTextureCoords[10]) * 0.0093f;

        gl_FragColor = sum;

    }
    else if(Mode == 0 || Mode == 1)
    {
        gl_FragColor =  texture2D(Texture,texCoordsOut)*finalColor;
    }

}
