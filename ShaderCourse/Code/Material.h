#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
class Material {
public:
	GLuint programHandle;

	void LoadFiles(const char* vertexPath, const char* fragmentPath);
	void Use() const;
	void Set(const char* name, const glm::mat4& value) const;
	void Set(const char* name, const glm::vec3& value) const;
	void Set(const char* name, const int value) const;
	void Set(const char* name, const float value) const;
	
};