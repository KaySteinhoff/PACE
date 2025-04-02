#include <PACEErrorHandling.h>
#include <PACE.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

unsigned int CreateTextureInstance(PATexture *tex)
{
	stbi_set_flip_vertically_on_load(1);

	if(!tex)
		return PACE_ERR_NULL_REFERENCE;

	glGenTextures(1, &tex->textureID);
	glBindTexture(GL_TEXTURE_2D, tex->textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	tex->width = 0;
	tex->height = 0;
	tex->nrChannels = 0;

	return PACE_ERR_SUCCESS;
}

unsigned int CreateTexture(PATexture *texture, int width, int height, int nrChannels, GLint byteFormat, GLenum format, unsigned char *data)
{
	if(!data)
		return PACE_ERR_NULL_REFERENCE;

	unsigned int err = 0;
	if((err = CreateTextureInstance(texture)))
		return err;

	glTexImage2D(GL_TEXTURE_2D, 0, byteFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	texture->width = width;
	texture->height = height;
	texture->nrChannels = nrChannels;

	return PACE_ERR_SUCCESS;
}

unsigned int LoadTextureFromFile(PATexture *texture, const char *path, GLint byteFormat, GLenum format)
{
	unsigned int err = 0;
	if((err = CreateTextureInstance(texture)))
		return err;

	unsigned char *data = stbi_load(path, &texture->width, &texture->height, &texture->nrChannels, 0);

	if(!data)
	{
		stbi_image_free(data);
		return PACE_ERR_INVALID_ARGUMENT;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, byteFormat, texture->width, texture->height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	return PACE_ERR_SUCCESS;
}
