#version 330 core
in vec2 TexCoords;
flat in int index;

out vec4 color;

uniform sampler2DArray text;
uniform int letterMap[128];
uniform vec3 textColor;

uniform float sdfThreshold;
uniform float edgeSmoothness;
uniform float outlineSize;
uniform vec3 outlineColor;

void main()
{
    float distance = texture(text, vec3(TexCoords.xy, letterMap[index])).r - sdfThreshold;

// Outline
    if (outlineSize > 0.0 && -outlineSize < distance && distance < 0.0) {
        // Not real antialiasing, but simulated via distance based alpha blending.
        float antialiasingFactor = smoothstep(-outlineSize - edgeSmoothness, -outlineSize + edgeSmoothness, distance);
        color = vec4(outlineColor, antialiasingFactor);
        // TODO: Add antialiasing to inside of outline.
    } else {
        float alpha = smoothstep(-edgeSmoothness, 0.0, distance);
        color = vec4(textColor, alpha);
    }
}