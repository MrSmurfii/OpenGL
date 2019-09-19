#include "Sphere.h"
#include <glm/glm.hpp>
#include "Transform.h"
#include "Material.h"

Sphere::Sphere(float radius /*= 1.0f*/, int sectorCount /*= 36*/, int stackCount /*= 18*/)
{
	SetSphereSize(radius, sectorCount, stackCount);
}

void Sphere::SetSphereSize(float inRadius, int inSectorCount, int inStackCount) {
	radius = inRadius;
	sectorCount = inSectorCount;
	if (inSectorCount < MIN_SECTOR_COUNT) {
		sectorCount = MIN_SECTOR_COUNT;
		printf("Sector count to small, auto set to: %d\n", MIN_SECTOR_COUNT);
	}
	stackCount = inStackCount;
	if(inStackCount < MIN_STACK_COUNT) {
		stackCount = MIN_STACK_COUNT;
		printf("Stack count to small, auto set to: %d\n", MIN_STACK_COUNT);
	}

	BuildVertecies();
}

void Sphere::DrawSphere(const Transform& transform, const Material& material) {
	mesh.Bind();
	material.Set("u_World", transform.GetMatrix());
	glDrawElements(GL_TRIANGLES, indexData.size(), GL_UNSIGNED_INT, (void*)0);
}

void Sphere::LoadSphere() {
	mesh.LoadVerts(sphereVertexData.data(), (unsigned int)sphereVertexData.size() * sizeof(float), indexData.data(), (unsigned int)indexData.size() * sizeof(unsigned int));
}

void Sphere::ClearVectors() {
	std::vector<float>().swap(sphereVertexData);
	std::vector<unsigned int>().swap(indexData);
}

void Sphere::BuildVertecies() {
	ClearVectors();

	float x, y, z;									// vertecies
	float nx, ny, nz, lengthInv = 1.0f / radius;	// normals
	float texA, texB;								// tex coords

	float sectorStep = 2 * PI / sectorCount;
	float stackStep = PI / stackCount;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stackCount; ++i) {
		stackAngle = PI / 2 - i * stackStep;
		z = radius * glm::sin(stackAngle);

		for (int j = 0; j <= sectorCount; ++j) {
			sectorAngle = j * sectorStep;

			// position of the vertecies
			x = (radius * glm::cos(stackAngle)) * glm::cos(sectorAngle);
			y = (radius * glm::cos(stackAngle)) * glm::sin(sectorAngle);
			sphereVertexData.push_back(x);
			sphereVertexData.push_back(y);
			sphereVertexData.push_back(z);

			// normals of the vertecies
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			sphereVertexData.push_back(nx);
			sphereVertexData.push_back(ny);
			sphereVertexData.push_back(nz);


			// tex coords
			texA = (float)j / sectorCount;
			texB = (float)i / stackCount;
			sphereVertexData.push_back(texA);
			sphereVertexData.push_back(texB);
		}
	}

	int index1, index2;
	for (int i = 0; i < stackCount; i++) {
		index1 = i * (sectorCount + 1);
		index2 = index1 + sectorCount + 1;

		for (int j = 0; j < sectorCount; j++, ++index1, ++index2) {
			if (i != 0){
				indexData.push_back(index1);
				indexData.push_back(index2);
				indexData.push_back(index1 + 1);
			}
			if(i != (stackCount-1)) {
				indexData.push_back(index1 + 1);
				indexData.push_back(index2);
				indexData.push_back(index2 + 1);
			}
		}
	}
}


