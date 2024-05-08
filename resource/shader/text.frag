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

    // The smoothstep function here adds an antialiasing effect to smooth out edges.
    float alpha = smoothstep(-edgeSmoothness, 0.0, distance);
    color = vec4(textColor, alpha);

    // Outline
    if (outlineSize > 0.0) {
        if (-outlineSize < distance && distance < 0.0) {
            // Not real antialiasing, but simulated via distance based alpha blending.
            float antialiasingFactor = smoothstep(-outlineSize - edgeSmoothness, -outlineSize + edgeSmoothness, distance);
            color = vec4(outlineColor, antialiasingFactor);
        }
        else if (0.0 <= distance && distance < edgeSmoothness) {
            float antialiasingFactor = smoothstep(edgeSmoothness, 0.0, distance);
            color = vec4(antialiasingFactor * outlineColor + (1.0 - antialiasingFactor) * textColor, 1.0);
        }
    }
}