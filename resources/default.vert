#version 150

in vec3 i_vert;

void main() {
    gl_Position = vec4(i_vert, 1);
}