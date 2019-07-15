#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 model;
uniform float pointSize;

out vec3 color;

void main() {
    gl_Position = model * vec4(aPos, 1.0);
    gl_PointSize = pointSize;
    color = aColor;
}
