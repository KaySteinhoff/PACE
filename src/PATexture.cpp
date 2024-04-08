#include <cppPACE.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int flippedOnLoad = 0;

PATexture::PATexture()
{
	if(!flippedOnLoad)
	{
		stbi_set_flip_vertically_on_load(1);
		flippedOnLoad = 1;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	width = 0;
	height = 0;
	nrChannels = 0;

	data = NULL;
}

PATexture* PATexture::LoadTextureFromFile(const char *path, GLint byteFormat, GLenum format)
{
	if(!flippedOnLoad)
	{
		stbi_set_flip_vertically_on_load(1);
		flippedOnLoad = 1;
	}

	PATexture *tex = (PATexture*)malloc(sizeof(PATexture));

	if(!tex)
		return NULL;

	glGenTextures(1, &tex->textureID);
	glBindTexture(GL_TEXTURE_2D, tex->textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	tex->data = stbi_load(path, &tex->width, &tex->height, &tex->nrChannels, 0);

	if(!tex->data)
	{
		printf("Failed to read image data!\n");
		stbi_image_free(tex->data);
		free(tex);
		return NULL;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, byteFormat, tex->width, tex->height, 0, format, GL_UNSIGNED_BYTE, tex->data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(tex->data);

	return tex;
}
