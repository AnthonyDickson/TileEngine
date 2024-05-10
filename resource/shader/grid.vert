#version 330 core

layout (location = 0) in vec2 position;

uniform mat4 projectionViewMatrix;
uniform mat4 transform;

void main() {
    gl_Position = projectionViewMatrix * transform * vec4(position.xy, 0.0, 1.0);
}