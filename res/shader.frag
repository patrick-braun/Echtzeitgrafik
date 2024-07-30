#version 330 core

in vec3 vertex_color;
out vec4 fragment_color;

void main() {
    fragment_color = vec4(vertex_color, 1.0f);
    //color = vec4(vec3(gl_FragCoord.z), 1.0);
}