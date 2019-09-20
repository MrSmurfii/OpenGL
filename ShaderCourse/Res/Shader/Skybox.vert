#version 330 core 
in vec3 a_Position;

uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 f_CubeDirection;

void main() 
{
	mat4 viewNoTranslate = u_View;
	viewNoTranslate[3] = vec4(0.0, 0.0, 0.0, 1.0);
    gl_Position = u_Projection * viewNoTranslate * vec4(a_Position, 1.0);
	gl_Position.z = gl_Position.w - 0.0001;
	f_CubeDirection = a_Position;
}