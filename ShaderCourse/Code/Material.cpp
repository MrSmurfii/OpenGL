#include "Material.h"
#include <stdio.h>
#include<glm/gtc/type_ptr.hpp>

GLuint LoadShader(const char* path, GLenum shaderType) {
	GLuint shaderHandle = glCreateShader(shaderType);

	FILE* file;
	fopen_s(&file, path, "rb");

	if (file == nullptr) {
		printf("shader file '%s' does not exist, moron...\n", path);
		return shaderHandle;
	}

	fseek(file, 0, SEEK_END);
	int fileLength = ftell(file);
	char* fileContents = new char[fileLength];
	fseek(file, 0, SEEK_SET);

	fread(fileContents, 1, fileLength, file);
	fclose(file);

	glShaderSource(shaderHandle, 1, &fileContents, &fileLength);
	glCompileShader(shaderHandle);

	delete[] fileContents;
	return shaderHandle;
}


void Material::LoadFiles(const char* vertexPath, const char* fragmentPath) {
	GLuint vertexHandle = LoadShader(vertexPath, GL_VERTEX_SHADER);
	GLuint fragmentHandle = LoadShader(fragmentPath, GL_FRAGMENT_SHADER);

	programHandle = glCreateProgram();
	glAttachShader(programHandle, vertexHandle);
	glAttachShader(programHandle, fragmentHandle);
	glLinkProgram(programHandle);


	glDetachShader(vertexHandle, programHandle);
	glDetachShader(fragmentHandle, programHandle);
	glDeleteShader(vertexHandle);
	glDeleteShader(fragmentHandle);

	// Error check
	GLint linkStatus;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &linkStatus);

	if (linkStatus == GL_FALSE) {
		printf("Oh oh, program failed to link, stupid!\n");
		char logBuffer[1024];
		glGetProgramInfoLog(programHandle, 1024, nullptr, logBuffer);

		printf("%s\n", logBuffer);
	}
}

void Material::Use() const {
	glUseProgram(programHandle);
}

void Material::Set(const char* name, const glm::mat4& value) const {
	Use();
	GLuint location = glGetUniformLocation(programHandle, name);
	glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));
}

void Material::Set(const char* name, const glm::vec3& value) const {
	Use();
	GLuint location = glGetUniformLocation(programHandle, name);
	glUniform3fv(location, 1, glm::value_ptr(value));
}

void Material::Set(const char* name, const int value) const {
	Use();
	GLuint location = glGetUniformLocation(programHandle, name);
	glUniform1i(location, value);
}

void Material::Set(const char* name, const float value) const {
	Use();
	GLuint location = glGetUniformLocation(programHandle, name);
	glUniform1f(location, value);
}
