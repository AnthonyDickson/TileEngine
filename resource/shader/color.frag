#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
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
in vec2 TextureCoordinates;

out vec4 FragColor;

uniform vec3 viewPosition;
uniform Material material;
uniform Light light;

void main() {
    vec3 ambient = light.ambient * texture(material.diffuse, TextureCoordinates).rgb;

    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(light.position - FragPosition);
    float diffuseAmount = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * diffuseAmount * texture(material.diffuse, TextureCoordinates).rgb;

    vec3 viewDirection = normalize(viewPosition - FragPosition);
    vec3 reflectDirection = reflect(-lightDirection, norm);
    float specularAmount = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    vec3 specular = light.specular * specularAmount * texture(material.specular, TextureCoordinates).rgb;

    FragColor = vec4((ambient + diffuse + specular), 1.0);
}
