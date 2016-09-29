#version 150

uniform sampler2D u_tex;
in vec2 o_fragTexCoord;
out vec4 o_finalColor;

void main() {
	o_finalColor = texture(u_tex, o_fragTexCoord);
	//o_finalColor = vec4(tmp.xyz, 1.0f);
	//o_finalColor = vec4(1.0f, 1.0f, 1.0f, 0.0f);
}