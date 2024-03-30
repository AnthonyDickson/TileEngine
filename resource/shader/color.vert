#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoordinates;

out vec3 FragPosition;
out vec3 Normal;
out vec2 TextureCoordinates;

uniform mat4 model;
uniform mat4 modelInverse;
uniform mat4 projectionViewMatrix;

void main() {
    gl_Position = projectionViewMatrix * model * vec4(position, 1.0);
    FragPosition = vec3(model * vec4(position, 1.0));
    Normal = mat3(modelInverse) * normal;
    TextureCoordinates = textureCoordinates;
}