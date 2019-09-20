#include "Mesh.h"

void Mesh::LoadVerts(const float* verts, int vertsSize, const unsigned int* indicies, int indexSize) {
	glGenVertexArrays(1, &vertexObject);
	glBindVertexArray(vertexObject);

	// load vertex data
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, vertsSize, verts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, (3 + 3 + 2) * sizeof(float), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, (3 + 3 + 2) * sizeof(float), (void*)((3) * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, false, (3 + 3 + 2) * sizeof(float), (void*)((3 + 3) * sizeof(float)));

	// load index data
	if (indicies != nullptr) {
		glGenBuffers(1, &indexBuffer);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indicies, GL_STATIC_DRAW);
	}

	glBindVertexArray(0);
}

void Mesh::Bind()
{
	glBindVertexArray(vertexObject);
}
