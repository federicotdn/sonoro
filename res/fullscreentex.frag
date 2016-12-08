#version 150

uniform sampler2D u_tex;
uniform uint u_ticks;

in vec2 o_fragTexCoord;
out vec4 o_finalColor;

/* Misc variables */
float offset = 1.0 / 100;
vec2 offsets[9] = vec2[](
	vec2(-offset, offset),  // top-left
	vec2(0.0f,    offset),  // top-center
	vec2(offset,  offset),  // top-right
	vec2(-offset, 0.0f),    // center-left
	vec2(0.0f,    0.0f),    // center-center
	vec2(offset,  0.0f),    // center-right
	vec2(-offset, -offset), // bottom-left
	vec2(0.0f,    -offset), // bottom-center
	vec2(offset,  -offset)  // bottom-right    
);

// Sharpen kernel
float sharpenKernel[9] = float[](
	-1, -1, -1,
	-1,  9, -1,
	-1, -1, -1
);

// Blur kernel
float blurKernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
);

// Edge detection kernel
float edgeKernel[9] = float[](
	1, 1, 1,
	1, -8, 1,
	1, 1, 1
);

/* End Misc variables */

void main() {

	/* Sample with kernel */
	vec3 sampleTex[9];
    for (int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(u_tex, o_fragTexCoord.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for (int i = 0; i < 9; i++)
	{
		col += sampleTex[i] * edgeKernel[i];
	}
	
	/* use kernel */
    o_finalColor = vec4(col, 1.0);

	/* No effect */
	//o_finalColor = texture(u_tex, o_fragTexCoord);
	
	/* Invert colors */
	//o_finalColor = vec4(vec3(1 - o_finalColor.x, 1 - o_finalColor.y, 1 - o_finalColor.z), 1.0);
	
	/* Grayscale */
	vec4 color = o_finalColor;
    float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    //o_finalColor = vec4(average, average, average, 1.0);
}