#version 330 core

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec4 ambientLight;

void main() {
    FragColor = vec4(ambientLight.a * ambientLight.rgb * objectColor, 1.0);
}