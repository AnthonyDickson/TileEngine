#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;

    vec3 color;
    float intensity;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;

    vec3 color;

    float linear;
    float quadratic;
};

struct PointLight {
    vec3 position;

    vec3 color;

    float linear;
    float quadratic;
};

in vec3 FragPosition;
in vec3 Normal;
in vec2 TextureCoordinates;

out vec4 FragColor;

#define POINT_LIGHT_COUNT 4

uniform vec3 viewPosition;
uniform Material material;
uniform DirectionalLight directionalLight;
uniform PointLight pointLight[POINT_LIGHT_COUNT];
uniform SpotLight spotLight;

vec3 calculateDirectionalLight(DirectionalLight light, vec3 norm);
vec3 calculatePointLight(PointLight light, vec3 norm);
vec3 calculateSpotLight(SpotLight light, vec3 norm);

void main() {
    vec3 norm = normalize(Normal);
    vec3 lighting = vec3(0.0);

    lighting += calculateDirectionalLight(directionalLight, norm);

    for (int i = 0; i < POINT_LIGHT_COUNT; i++) {
        lighting += calculatePointLight(pointLight[i], norm);
    }

    lighting += calculateSpotLight(spotLight, norm);

    FragColor = vec4(lighting, 1.0);
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

float calculateAttenuation(vec3 position, float linear, float quadratic) {
    float distance = length(position - FragPosition);
    return 1.0 / (1.0 + linear * distance + quadratic * pow(distance, 2));
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 norm) {
    vec3 lightDirection = normalize(-light.direction);
    vec3 diffuse = calculateDiffuse(light.intensity * light.color, norm, lightDirection);
    vec3 specular = calculateSpecular(light.intensity * light.color, norm, lightDirection);

    return diffuse + specular;
}

vec3 calculatePointLight(PointLight light, vec3 norm) {
    vec3 lightDirection = normalize(light.position - FragPosition);
    vec3 diffuse = calculateDiffuse(light.color, norm, lightDirection);
    vec3 specular = calculateSpecular(light.color, norm, lightDirection);
    float attenuation = calculateAttenuation(light.position, light.linear, light.quadratic);

    return attenuation * (diffuse + specular);
}

vec3 calculateSpotLight(SpotLight light, vec3 norm) {
    vec3 lightDirection = normalize(light.position - FragPosition);
    float theta = dot(lightDirection, normalize(-light.direction));

    if (theta < light.outerCutOff) {
        return vec3(0.0);
    }

    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    float attenuation = calculateAttenuation(light.position, light.linear, light.quadratic);

    vec3 diffuse = calculateDiffuse(light.color, norm, lightDirection);
    vec3 specular = calculateSpecular(light.color, norm, lightDirection);

    return intensity * attenuation * (diffuse + specular);
}
