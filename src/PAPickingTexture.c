#include <stdlib.h>
#include <PACE.h>

//TODO:Implement picking technique
PAPickingTexture* CreatePickingTexture(uint32_t width, uint32_t height)
{
	PAPickingTexture *tex = malloc(sizeof(PAPickingTexture));
	if(!tex)
		return NULL;

	glGenBuffers(1, &tex->pbo);
	glGenFramebuffers(1, &tex->fbo);

	glGenTextures(1, &tex->pickingTexture);
	glBindTexture(GL_TEXTURE_2D, tex->pickingTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32UI, width, height, 0, GL_RGB_INTEGER, GL_UNSIGNED_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->pickingTexture, 0);

	glGenTextures(1, &tex->depthTexture);
	glBindTexture(GL_TEXTURE_2D, tex->depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex->depthTexture, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if(status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Failed to create picking texture framebuffer. Status: 0x%x\n", status);
		free(tex);
		return NULL;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return NULL;
}

PixelInfo ReadPixelInfo(PAPickingTexture *papickingTexture, uint32_t x, uint32_t y)
{
	PixelInfo info = (PixelInfo){ 0 };
	if(x < 0 || y < 0 || x > papickingTexture->width || y > papickingTexture->height)
		return info;

	glBindFramebuffer(GL_READ_FRAMEBUFFER, papickingTexture->fbo);

	glReadBuffer(GL_COLOR_ATTACHMENT0);

	glBindBuffer(GL_PIXEL_PACK_BUFFER, papickingTexture->pbo);
	glReadPixels(x, y, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, &info);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	return info;
}
