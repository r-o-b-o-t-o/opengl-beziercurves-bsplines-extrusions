#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 colorOverride;

out vec3 color;

void main() {
    gl_Position = projection * view * model * vec4(aPos.xyz, 1.0);
    if (colorOverride.r == 0.0f && colorOverride.g == 0.0f && colorOverride.b == 0.0f) {
        color = aColor;
    } else {
        color = colorOverride;
    }
}
