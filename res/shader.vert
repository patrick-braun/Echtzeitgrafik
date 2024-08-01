#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_uv;

uniform mat4 u_model;
uniform mat4 u_screenSpaceTransform;

out vec3 normal;
out vec3 fragPos;
out vec2 uv;

void main() {
    normal = mat3(u_model) * in_normal;
    fragPos = vec3(u_model * vec4(in_position, 1.0));

    gl_Position = u_screenSpaceTransform * vec4(in_position, 1.0);
    uv = in_uv;
}