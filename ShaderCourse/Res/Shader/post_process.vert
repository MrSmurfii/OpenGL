#version 330 core

const vec2 quadVerts[] = vec2[] (
vec2(-1.0, -1.0),
vec2(1.0, -1.0),
vec2(1.0, 1.0),
vec2(-1.0, 1.0)
);

const vec2 quadTexCoords[] = vec2[](
vec2(0.0, 0.0),
vec2(1.0, 0.0),
vec2(1.0, 1.0),
vec2(0.0, 1.0)
);

out vec2 f_TexCoord;

void main(){
	gl_Position = vec4(quadVerts[gl_VertexID], 0.0, 1.0);
	f_TexCoord = quadTexCoords[gl_VertexID];
}