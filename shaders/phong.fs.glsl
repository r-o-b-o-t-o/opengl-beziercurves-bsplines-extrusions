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

in vec3 normal;
in vec3 fragPos;

out vec4 fragColor;

uniform vec3 viewPosition;
uniform Material material;
uniform Light light;

void main() {
    vec3 ambient = material.ambient * light.ambient;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * material.diffuse * light.diffuse;

    vec3 viewDir = normalize(viewPosition - fragPos);
    vec3 reflectionDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectionDir), 0.0f), material.shininess);
    vec3 specular = spec * material.specular * light.specular;

    vec3 result = ambient + diffuse + specular;
    fragColor = vec4(result.rgb, 1.0);
}
