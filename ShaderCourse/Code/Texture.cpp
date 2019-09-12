#include "Texture.h"
#include <SOIL.h>
#include <cstring>

void SwizzleRows(unsigned char* data, int width, int height) {
	unsigned char* temp = new unsigned char[width * 4];
	int numSwaps = height * 0.5f;

	for (int i = 0; i < numSwaps; ++i) {
		unsigned char* sourcePtr = data + (width * 4) * i;
		unsigned char* targetPtr = data + (width * 4) * (height - i - 1);

		memcpy(temp, sourcePtr, width * 4);
		memcpy(sourcePtr, targetPtr, width * 4);
		memcpy(targetPtr, temp, width * 4);
	}

	delete[] temp;
}

void Texture::LoadFile(const char* file)
{
	int width;
	int height;
	int channels;
	unsigned char* imagePixels = SOIL_load_image(file, &width, &height, &channels, SOIL_LOAD_RGBA);
	SwizzleRows(imagePixels, width, height);

	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imagePixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	SOIL_free_image_data(imagePixels);
}

void Texture::Bind(int index)
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, handle);
}
