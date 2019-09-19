#version 330 core

out vec4 o_Color;

in vec3 f_Position;
in vec3 f_Normal;

uniform vec3 cameraPosition;
uniform samplerCube skybox;

void main() {
	vec3 eyeDirection = normalize(f_Position - cameraPosition);
	vec3 reflection = reflect(eyeDirection, normalize(f_Normal));

	o_Color = vec4(texture(skybox, reflection).rgb, 1.0);
}	