#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPosition;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 viewPosition;
uniform Material material;
uniform Light light;

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(light.position - FragPosition);
    float diffuseAmount = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = (diffuseAmount * material.diffuse) * light.diffuse;

    vec3 ambient = material.ambient * light.ambient;

    vec3 viewDirection = normalize(viewPosition - FragPosition);
    vec3 reflectDirection = reflect(-lightDirection, norm);
    float specularAmount = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    vec3 specular = (material.specular * specularAmount) * light.specular;

    FragColor = vec4((ambient + diffuse + specular), 1.0);
}