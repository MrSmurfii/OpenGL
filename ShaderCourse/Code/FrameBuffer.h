#pragma once
#include "Texture.h"
#include "GL/glew.h"

class FrameBuffer {
public:
	GLuint handle;
	Texture colorTexture;
	Texture depthTexture;

	int width, height;

	void Create(int inWidth, int inHeight);
	void Bind();
	void Unbind();
};