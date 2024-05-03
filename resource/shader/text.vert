#version 330 core
layout (location = 0) in vec2 position;

out vec2 TexCoords;
flat out int index;

uniform mat4 transforms[128];
uniform mat4 projection;

void main()
{
    gl_Position = projection * transforms[gl_InstanceID] * vec4(position.xy, 0.0, 1.0);
    index = gl_InstanceID;
    TexCoords.x = position.x;
    // Flip y coordinate since +y points down for position, but we want +y to point up for the UV coordinates.
    TexCoords.y = 1.0f - position.y;
}