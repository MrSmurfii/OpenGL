#pragma once
#include "GL/glew.h"

class CubeMap{
public:
	GLuint handle;
	void LoadFiles(const char** paths);
	void Bind(int index = 0);
};