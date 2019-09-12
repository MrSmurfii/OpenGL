#version 330 core

uniform sampler2D u_FrameColor;
uniform sampler2D u_FrameDepth;

in vec2 f_TexCoord;
out vec4 o_Color;
const vec3 fogColor = vec3(0.3, 0.5, 0.7);

void main(){
	vec4 color = texture(u_FrameColor, f_TexCoord);
	float depth = texture(u_FrameDepth, f_TexCoord).x;
	depth = pow(depth, 500.0) * 0.5;

	color.xyz = mix(color.xyz, fogColor, depth);
	o_Color = color;
}