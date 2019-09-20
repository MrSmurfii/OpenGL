#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;

out vec3 f_Normal;
out vec3 f_Position;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_World;

void main() {
	f_Normal = mat3(transpose(inverse(u_World))) * a_Normal;
	f_Position = vec3(u_World * vec4(a_Position, 1.0));
	gl_Position = u_Projection * u_View * u_World * vec4(a_Position, 1.0);
}