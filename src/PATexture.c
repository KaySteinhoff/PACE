#include <PACE.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

PATexture* CreateTextureInstance()
{
	stbi_set_flip_vertically_on_load(1);

	PATexture *tex = malloc(sizeof(PATexture));

	if(!tex)
	{
		printf("Failed to allocate texture\n");
		return NULL;
	}

	glGenTextures(1, &tex->textureID);
	glBindTexture(GL_TEXTURE_2D, tex->textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	tex->width = 0;
	tex->height = 0;
	tex->nrChannels = 0;
	tex->data = NULL;

	return tex;
}

PATexture* CreateTexture(int width, int height, int nrChannels, unsigned char *data)
{
	if(!data)
		return NULL;
	PATexture *tex = CreateTextureInstance();

	if(!tex)
		return NULL;

	tex->data = data;

	return tex;
}

PATexture* LoadTextureFromFile(const char *path, GLint byteFormat, GLenum format)
{
	PATexture *tex = CreateTextureInstance();

	if(!tex)
	{
		printf("No texture instance could be created\n");
		return NULL;
	}

	tex->data = stbi_load(path, &tex->width, &tex->height, &tex->nrChannels, 0);

	if(!tex->data)
	{
		printf("Failed to read image data\n");
		stbi_image_free(tex->data);
		free(tex);
		return NULL;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, byteFormat, tex->width, tex->height, 0, format, GL_UNSIGNED_BYTE, tex->data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(tex->data);

	return tex;
}
