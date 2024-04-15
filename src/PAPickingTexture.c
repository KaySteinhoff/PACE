#include <stdlib.h>
#include <PACE.h>

//TODO:Implement picking technique
PAPickingTexture* CreatePickingTexture(uint32_t width, uint32_t height)
{
	PAPickingTexture *papickingTexture = malloc(sizeof(PAPickingTexture));

	if(!papickingTexture)
	{
		printf("Failed to allocate picking texture!\n");
		return NULL;
	}

	//Generate framebuffer
	glGenFramebuffers(1, &papickingTexture->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, papickingTexture->fbo);

	//Generate integer texture
	glGenTextures(1, &papickingTexture->pickingTexture);
	glBindTexture(GL_TEXTURE_2D, papickingTexture->pickingTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32UI, width, height, 0, GL_RGB_INTEGER, GL_UNSIGNED_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Generate custom depth texture
	glGenTextures(1, &papickingTexture->depthTexture);
	glBindTexture(GL_TEXTURE_2D, papickingTexture->depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, papickingTexture->depthTexture, 0);

	//Get framebuffer status
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	//If framebuffer incomplete free picking texture and return NULL
	if(status != GL_FRAMEBUFFER_COMPLETE)
	{
		//Bind back the default framebuffer
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		printf("Failed to create framebuffer for the picking texture! Status: %d\n", status);
		free(papickingTexture);
		return NULL;
	}

	//Bind back the default framebuffer
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	papickingTexture->pickingShader = LoadShaderFromSource("shaders/picking.vert", "shaders/picking.frag");

	//Otherwise return created picking texture
	return papickingTexture;
}

void EnablePickingTexture(PAPickingTexture *papickingTexture)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, papickingTexture->fbo);
}

void DisablePickingTexture(PAPickingTexture *papickingTexture)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void ReadPixelInfo(PAPickingTexture *papickingTexture, uint32_t x, uint32_t y)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, papickingTexture->fbo);

	glReadBuffer(GL_COLOR_ATTACHMENT0);

	glReadPixels(x, y, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, &papickingTexture->pixelInfo);

	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

void DestroyPickingTexture(PAPickingTexture *papickingTexture)
{
	if(papickingTexture->fbo != 0)
		glDeleteFramebuffers(1, &papickingTexture->fbo);
	if(papickingTexture->pickingTexture != 0)
		glDeleteTextures(1, &papickingTexture->pickingTexture);
	if(papickingTexture->depthTexture != 0)
		glDeleteTextures(1, &papickingTexture->depthTexture);
}

void PickObjects(PACamera *camera, PAPickingTexture *papickingTexture, PAMesh **meshes, int numMeshes)
{
	EnablePickingTexture(papickingTexture);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(papickingTexture->pickingShader->ID);

	for(int i = 0; i < numMeshes; ++i)
	{
		mat4x4_apply_transform(meshes[i]->transform.transformMatrix, &meshes[i]->transform);

		glUniform1ui(glGetUniformLocation(papickingTexture->pickingShader->ID, "gObjectIndex"), i + 1);
		glUniformMatrix4fv(papickingTexture->pickingShader->modelLocation, 1, GL_FALSE, (const GLfloat*)meshes[i]->transform.transformMatrix);
		glUniformMatrix4fv(papickingTexture->pickingShader->viewLocation, 1, GL_FALSE, (const GLfloat*)camera->transform.transformMatrix);
		if(camera->viewMode == PAProjection)
			glUniformMatrix4fv(papickingTexture->pickingShader->perspectiveLocation, 1, GL_FALSE, (const GLfloat*)camera->projectionMatrix);
		else
			glUniformMatrix4fv(papickingTexture->pickingShader->perspectiveLocation, 1, GL_FALSE, (const GLfloat*)camera->orthoMatrix);

		glBindVertexArray(meshes[i]->vao);
		glDrawArrays(GL_TRIANGLES, 0, meshes[i]->numFaces);
	}

	DisablePickingTexture(papickingTexture);
}
