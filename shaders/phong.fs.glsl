#version 330 core

#define MAX_LIGHTS 10

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
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

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 normal;
in vec3 fragPos;

out vec4 fragColor;

uniform vec3 viewPosition;
uniform Material material;

uniform int nbPointLights;
uniform PointLight[MAX_LIGHTS] pointLights;

uniform int nbDirectionalLights;
uniform DirectionalLight[MAX_LIGHTS] directionalLights;

vec3 computePointLight(PointLight light) {
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = material.ambient * light.ambient;
    ambient *= attenuation;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * material.diffuse * light.diffuse;
    diffuse *= attenuation;

    vec3 viewDir = normalize(viewPosition - fragPos);
    vec3 reflectionDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectionDir), 0.0f), material.shininess);
    vec3 specular = spec * material.specular * light.specular;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

vec3 computeDirectionalLight(DirectionalLight light) {
    vec3 ambient = material.ambient * light.ambient;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * material.diffuse * light.diffuse;

    vec3 viewDir = normalize(viewPosition - fragPos);
    vec3 reflectionDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectionDir), 0.0f), material.shininess);
    vec3 specular = spec * material.specular * light.specular;

    return ambient + diffuse + specular;
}

void main() {
    vec3 output = vec3(0.0);
    for (int i = 0; i < min(nbPointLights, MAX_LIGHTS); ++i) {
        output += computePointLight(pointLights[i]);
    }
    for (int i = 0; i < min(nbDirectionalLights, MAX_LIGHTS); ++i) {
        output += computeDirectionalLight(directionalLights[i]);
    }
    fragColor = vec4(output.rgb, 1.0);
}
