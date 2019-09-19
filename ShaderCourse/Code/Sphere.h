#pragma once
#include <vector>
#include "Mesh.h"

class Transform;
class Material;

class Sphere {
public:
	Sphere(float radius = 1.0f, int sectorCount = 36, int stackCount = 18);
	~Sphere() {}

	void SetSphereSize(float inRadius, int inSectorCount, int inStackCount);
	void DrawSphere(const Transform& transform, const Material& material);
	void LoadSphere();

private:
	const int MIN_SECTOR_COUNT = 3;
	const int MIN_STACK_COUNT = 2;
	const float PI = 3.1415926f;

	float radius;
	int sectorCount;
	int stackCount;
	bool smooth;
	std::vector<float> sphereVertexData;
	std::vector<unsigned int> indexData;

	Mesh mesh;

	void BuildVertecies();
	void ClearVectors();
};