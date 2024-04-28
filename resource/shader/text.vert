#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 textureCoords;
out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(position.xy, 1.0, 1.0);
    TexCoords = textureCoords;
}