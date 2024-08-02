#version 330 core

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
    float constant;
    float linear;
    float quadratic;
};

in vec3 normal;
in vec3 fragPos;
in vec3 vertex_color;
in vec2 uv;

uniform PointLight u_light;
uniform vec3 u_viewPos;
uniform sampler2D u_img;
uniform bool u_selfEmitting;

out vec4 out_color;

void main() {
    vec3 objColor = texture(u_img, uv).rgb;
    if (u_selfEmitting) {
        out_color = vec4(objColor, 1.0);
        return;
    }

    float ambientStr = 0.4;
    vec3 ambientLightColor = vec3(1.0);
    vec3 ambient = ambientStr * ambientLightColor;

    vec3 lightColor = u_light.color * u_light.intensity;
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(u_light.position - fragPos);
    vec3 diffuse = max(dot(norm, lightDir), 0.0) * lightColor;

    float specularStr = 0.15;
    vec3 viewDir = normalize(u_viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 spec = specularStr * max(dot(norm, lightDir), 0.0) * pow(max(dot(viewDir, reflectDir), 0.0001), 32) * lightColor;

    float dist = length(u_light.position - fragPos);
    float attenuation = 1.0 / (
    u_light.constant +
    u_light.linear * dist +
    u_light.quadratic * pow(dist, 2.0)
    );
    ambient *= attenuation;
    diffuse *= attenuation;
    spec *= attenuation;
    out_color = vec4((ambient + diffuse + spec) * objColor, 1.0);
}