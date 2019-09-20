#pragma once
#include <glm/glm.hpp>

class Material;

class SpotLight {
public:
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 color;
	float radius;
	float angle;

	void UploadToMaterial(const Material& targetMaterial);
};