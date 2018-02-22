/* This shader takes a vertex position from the texture and outputs it as a vertex */

#version 330

precision highp float;

uniform mat4 modelViewProjectionMatrix;
in vec2 texcoord;
uniform sampler2DRect posTexture;
uniform vec2 screen;
uniform float PI, TWO_PI, res;

void main() {
    /* Read position from the texture */
    vec4 pos = texture(posTexture, texcoord);

    /* Map the position into screen space */
    pos.x *= screen.x / PI;
    pos.y *= screen.y / PI;

    /* Output the vertex */
    gl_Position = modelViewProjectionMatrix * pos;
}
