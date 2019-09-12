#include "Transform.h"
#include <glm/gtx/matrix_decompose.hpp>

glm::mat4 Transform::GetMatrix() const
{
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.f), scale);
	glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
	glm::mat4 positionMatrix = glm::translate(glm::mat4(1.f), position);

	return positionMatrix * rotationMatrix * scaleMatrix;
}

Transform Transform::operator*(const Transform& other) const{
	glm::mat4 myMatrix = GetMatrix();
	glm::mat4 otherMatrix = other.GetMatrix();

	glm::mat4 resultMatrix = myMatrix * otherMatrix;
	
	Transform resultTransform;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(resultMatrix, resultTransform.scale, resultTransform.rotation, resultTransform.position, skew, perspective);

	return resultTransform;
}
