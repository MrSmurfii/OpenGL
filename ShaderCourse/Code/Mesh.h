#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>

class Mesh {
public:
	GLuint vertexObject;
	GLuint buffer;
	GLuint indexBuffer;

	void LoadVerts(const float* verts, int vertsSize, const unsigned int* indicies, int indexSize);
	void Bind();
};