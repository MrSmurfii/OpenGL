#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
class Material {
public:
	GLuint programHandle;

	void LoadFiles(const char* vertexPath, const char* fragmentPath);
	void Use();
	void Set(const char* name, const glm::mat4& value);
	void Set(const char* name, const glm::vec3& value);
	void Set(const char* name, const int value);

};