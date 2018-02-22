/* This shader just colours the vertex and sets the alpha channel, not really doing much */

#version 330

precision highp float;

out vec4 outputColor;

void main() {
    outputColor = vec4(vec3(0.0), 0.004);
}
