#version 330 core

layout (location = 0) in vec2 position;

out vec2 TexCoord;

uniform mat4 projectionViewMatrix;
uniform mat4 transforms[128];
uniform vec2 textureCoordinates[128];
uniform vec2 tileSize;

void main() {
    gl_Position = projectionViewMatrix * transforms[gl_InstanceID] * vec4(position.xy, 0.0, 1.0);
    TexCoord = textureCoordinates[gl_InstanceID].xy + tileSize.xy * position.xy;
}