#version 330 core

in vec3 FragPosition;
in vec3 Normal;

out vec4 FragColor;

uniform vec4 ambientLight;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPosition;
uniform float specularStrength;

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(lightPosition - FragPosition);
    float diffuseAmount = max(dot(norm, lightDirection), 0.0);

    vec3 ambient = ambientLight.rgb * ambientLight.a;
    vec3 diffuse = diffuseAmount * lightColor;

    vec3 viewDirection = normalize(viewPosition - FragPosition);
    vec3 reflectDirection = reflect(-lightDirection, norm);
    float specularAmount = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
    vec3 specular = specularStrength * specularAmount * lightColor;

    FragColor = vec4((ambient + diffuse + specular) * objectColor, 1.0);
}