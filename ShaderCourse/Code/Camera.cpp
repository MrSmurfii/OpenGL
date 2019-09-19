#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtx\quaternion.hpp>

glm::vec3 Camera::GetForwardVector(){
	glm::quat yawQuat = glm::angleAxis(glm::radians(yaw), glm::vec3(0.f, 1.f, 0.f));
	glm::quat pitchQuat = glm::angleAxis(glm::radians(pitch), glm::vec3(1.f, 0.f, 0.f));

	glm::quat finalQuat = yawQuat * pitchQuat;
	return finalQuat * glm::vec3(0.f, 0.f, -1.f);
}

glm::vec3 Camera::GetRightVector(){
	return glm::normalize(glm::cross(GetForwardVector(), glm::vec3(0.f, 1.f, 0.f)));
}

glm::mat4 Camera::GetViewMatrix(){
	return glm::lookAt(position, position + GetForwardVector(), glm::vec3(0.f, 1.f, 0.f));
}

void Camera::AddYaw(float yawAngle){
	yaw += yawAngle;
}

void Camera::AddPitch(float pitchAngle){
	pitch += pitchAngle;
	pitch = glm::clamp(pitch, -85.f, 85.f);
}

Transform Camera::GetTransform() const {
	Transform result;
	result.position = position;
	glm::quat yawQuat = glm::angleAxis(glm::radians(yaw), glm::vec3(0.f, 1.f, 0.f));
	glm::quat pitchQuat = glm::angleAxis(glm::radians(pitch), glm::vec3(1.f, 0.f, 0.f));
	result.rotation = yawQuat * pitchQuat;

	return result;
}
