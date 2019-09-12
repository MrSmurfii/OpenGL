#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

const glm::quat quat_identity = glm::identity<glm::quat>();

class Transform {
public:
	glm::vec3 position = glm::vec3(0.f);
	glm::quat rotation = glm::identity<glm::quat>();
	glm::vec3 scale = glm::vec3(1.f);

	Transform() {}
	Transform(const glm::vec3& position) : position(position) {}
	Transform(const glm::vec3& position, const glm::quat& rotation) : position(position), rotation(rotation) {}
	Transform(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale) :
		position(position), rotation(rotation), scale(scale) {}


	glm::mat4 GetMatrix() const;

	Transform operator*(const Transform& other) const;
};