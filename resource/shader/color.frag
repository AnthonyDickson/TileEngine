#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

in vec3 FragPosition;
in vec3 Normal;

out vec4 FragColor;

uniform vec4 ambientLight;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 viewPosition;
uniform Material material;

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(lightPosition - FragPosition);
    float diffuseAmount = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = (diffuseAmount * material.diffuse) * lightColor;

    vec3 ambient = material.ambient * ambientLight.a;

    vec3 viewDirection = normalize(viewPosition - FragPosition);
    vec3 reflectDirection = reflect(-lightDirection, norm);
    float specularAmount = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    vec3 specular = (material.specular * specularAmount) * lightColor;

    FragColor = vec4((ambient + diffuse + specular), 1.0);
}