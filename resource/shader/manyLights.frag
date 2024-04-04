#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPosition;
in vec3 Normal;
in vec2 TextureCoordinates;

out vec4 FragColor;

uniform vec3 viewPosition;
uniform Material material;
uniform DirectionalLight directionalLight;
uniform PointLight pointLight;

vec3 calculateDirectionalLight(DirectionalLight light);
vec3 calculatePointLight(PointLight light);

void main() {
    vec3 directionalLightEffect = calculateDirectionalLight(directionalLight);
    vec3 pointLightEffect = calculatePointLight(pointLight);

    FragColor = vec4((directionalLightEffect + pointLightEffect), 1.0);
}

vec3 calculateDiffuse(vec3 diffuse, vec3 norm, vec3 lightDirection) {
    float diffuseAmount = max(dot(norm, lightDirection), 0.0);
    return diffuse * diffuseAmount * texture(material.diffuse, TextureCoordinates).rgb;
}

vec3 calculateSpecular(vec3 specular, vec3 norm, vec3 lightDirection) {
    vec3 viewDirection = normalize(viewPosition - FragPosition);
    vec3 reflectDirection = reflect(-lightDirection, norm);
    float specularAmount = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    return specular * specularAmount * texture(material.specular, TextureCoordinates).rgb;
}

vec3 calculateDirectionalLight(DirectionalLight light) {
    vec3 ambient = light.ambient * texture(material.diffuse, TextureCoordinates).rgb;

    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(-light.direction);
    vec3 diffuse = calculateDiffuse(light.diffuse, norm, lightDirection);
    vec3 specular = calculateSpecular(light.specular, norm, lightDirection);

    return ambient + diffuse + specular;
}

vec3 calculatePointLight(PointLight light) {
    vec3 ambient = light.ambient * texture(material.diffuse, TextureCoordinates).rgb;

    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(light.position - FragPosition);
    vec3 diffuse = calculateDiffuse(light.diffuse, norm, lightDirection);
    vec3 specular = calculateSpecular(light.specular, norm, lightDirection);

    vec3 lighting = ambient + diffuse + specular;

    float distance = length(light.position - FragPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * pow(distance, 2));

    return attenuation * lighting;
}
