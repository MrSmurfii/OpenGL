#pragma once
#include <glm/glm.hpp>

class Camera {
public:

	glm::vec3 position = glm::vec3(0.f);
	float yaw = 0.f;
	float pitch = 0.f;

	glm::vec3 GetForwardVector();
	glm::vec3 GetRightVector();
	glm::mat4 GetViewMatrix();
	void AddYaw(float yawAngle);
	void AddPitch(float pitchAngle);
};