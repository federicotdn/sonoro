#version 150

in vec3 i_vert;
in vec2 i_vertTexCoord;
out vec2 o_fragTexCoord;

void main() {
	o_fragTexCoord = i_vertTexCoord;
    gl_Position = vec4(i_vert, 1);
}