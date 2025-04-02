#include <PACEGraphics.h>
#include <PACEErrorHandling.h>
#include <PACE.h>
#include <stdarg.h>

#define INVALID_UNIFORM_LOCATION 0xffffffff

extern PATexture defaultPACETexture;
extern int LogShaderCompile(GLint shaderID);

unsigned int EnableMaterial(PAMaterial *material)
{
	if(!material->ID)
		PACE_ERR_NULL_REFERENCE; // Although its not a NULL pointer it is getting treated like one, since the shader id refers to a shader object
	glUseProgram(material->ID);
}

unsigned int SetInt(PAMaterial *material, const char *name, int value)
{
	if(!material)
		return PACE_ERR_NULL_REFERENCE;
	GLint location = glGetUniformLocation(material->ID, name);

	if(location == INVALID_UNIFORM_LOCATION)
		return PACE_ERR_INVALID_ARGUMENT;

	glUniform1i(location, value);
	return PACE_ERR_SUCCESS;
}

unsigned int SetFloat(PAMaterial *material, const char *name, float value)
{
	if(!material)
		return PACE_ERR_NULL_REFERENCE;
	GLint location = glGetUniformLocation(material->ID, name);

	if(location == INVALID_UNIFORM_LOCATION)
		return PACE_ERR_INVALID_ARGUMENT;

	glUniform1f(location, value);
	return PACE_ERR_SUCCESS;
}

unsigned int CreatePAMaterial(PAMaterial *material, int n, ...)
{
	if(!material)
		return PACE_ERR_NULL_REFERENCE;

	glGenBuffers(1, &material->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, material->vbo);

	material->ID = glCreateProgram();
	va_list shaders;
	va_start(shaders, n);
	for(int i = 0; i < n; ++i)
		glAttachShader(material->ID, va_arg(shaders, PAShader));
	glLinkProgram(material->ID);

	if(!LogShaderCompile(material->ID))
		return PACE_ERR_FAILURE;

	material->modelLocation = glGetUniformLocation(material->ID, "model");
	material->viewLocation = glGetUniformLocation(material->ID, "view");
	material->perspectiveLocation = glGetUniformLocation(material->ID, "perspective");

	if(material->modelLocation == INVALID_UNIFORM_LOCATION)
		printf("Failed to find model matrix location\n");
	if(material->viewLocation == INVALID_UNIFORM_LOCATION)
		printf("Failed to find view matrix location\n");
	if(material->perspectiveLocation == INVALID_UNIFORM_LOCATION)
		printf("Failed to find perspective matrix location\n");

	material->texture = &defaultPACETexture;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return PACE_ERR_SUCCESS;
}

unsigned int PAMaterialSetPATexture(PAMaterial *material, PATexture *texture)
{
	if(!material || !texture)
		return PACE_ERR_NULL_REFERENCE;

	material->texture = texture;
}
