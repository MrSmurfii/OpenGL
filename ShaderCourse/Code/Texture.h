#pragma once
#include <GL/glew.h>

class Texture {
public:

	GLuint handle;

	
	void LoadFile(const char* file);
	void Bind(int index = 0);
};