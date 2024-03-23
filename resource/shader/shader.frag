#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D textureSampler1;
uniform sampler2D textureSampler2;

void main() {
    FragColor = mix(texture(textureSampler1, TexCoord), texture(textureSampler2, TexCoord), 0.2);
}