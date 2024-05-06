#version 330 core
in vec2 TexCoords;
flat in int index;

out vec4 color;

uniform sampler2DArray text;
uniform int letterMap[128];
uniform vec3 textColor;

void main()
{
    vec4 textureColor = texture(text, vec3(TexCoords.xy, letterMap[index]));
    float alpha = textureColor.r > 0.5 ? 1.0 : 0.0;
    vec4 sampled = vec4(1.0, 1.0, 1.0, alpha);
    color = vec4(textColor, 1.0) * sampled;
}