#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texCoord;
uniform sampler2D CC_Texture0;
uniform vec2 resolution;
uniform float pixelSize;

void main() {
    vec2 uv = v_texCoord;
    vec2 pixel = pixelSize / resolution;
    uv = floor(uv / pixel) * pixel + pixel * 0.75;
    gl_FragColor = texture2D(CC_Texture0, uv);
}
