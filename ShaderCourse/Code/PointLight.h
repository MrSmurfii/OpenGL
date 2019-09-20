#pragma once
#include <glm/glm.hpp>

class Material;

class PointLight {
	public:
		glm::vec3 position;
		glm::vec3 color;
		float radius;

		void UploadToMaterial(int index, const Material& targetMaterial);
};