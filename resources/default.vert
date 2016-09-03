#version 150

uniform mat4 u_camera;

in vec3 i_vert;

void main() {
    gl_Position = u_camera * vec4(i_vert, 1);
	//gl_Position = vec4(i_vert, 1);
}