#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_color;
layout (location = 2) in vec3 in_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 normal;
out vec3 fragPos;
out vec3 vertex_color;

void main() {
    normal = mat3(u_model) * in_normal;
    fragPos = vec3(u_model * vec4(in_position, 1.0));

    gl_Position = u_projection * u_view * u_model * vec4(in_position, 1.0);
    vertex_color = in_color;
}