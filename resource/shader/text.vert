#version 330 core
layout (location = 0) in vec2 position;
out vec2 TexCoords;

uniform mat4 transform;
uniform mat4 projection;

void main()
{
    gl_Position = projection * transform * vec4(position.xy, 0.0, 1.0);
    TexCoords.x = position.x;
    // Flip y coordinate since +y points down for position, but we want +y to point up for the UV coordinates.
    TexCoords.y = 1.0f - position.y;
}