#include <PACEErrorHandling.h>
#include <PACEGraphics.h>
#include <stdio.h>
#include <GL/gl.h>

unsigned int LoadShaderFromSource(PAShader *shader, GLenum shaderType, char *path)
{
	if(!shader || !path)
		return PACE_ERR_NULL_REFERENCE;

	FILE *fptr = fopen(path, "r");

	if(!fptr)
		return PACE_ERR_INVALID_ARGUMENT;

	fseek(fptr, 0, SEEK_END);
	long fsize = ftell(fptr);
	fseek(fptr, 0, SEEK_SET);

	char source[fsize+1];
	source[fsize] = '\0';

	fread(source, 1, fsize, fptr);
	fclose(fptr);

	return CompileShader(shader, shaderType, (const char*)source);
}

int LogShaderCompile(GLuint shaderID)
{
	int success;
	char log[512];
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

	glGetShaderInfoLog(shaderID, 512, NULL, log);
	if(!success)
		ThrowError(log, "ERROR: SHADER_COMPILATION_FAILED");

	return success;
}

unsigned int CompileShader(PAShader *shader, GLenum shaderType, const char *shaderSource)
{
	if(!shader || !shaderSource)
		return PACE_ERR_NULL_REFERENCE;

	*shader = glCreateShader(shaderType);
	glShaderSource(*shader, 1, &shaderSource, NULL);
	glCompileShader(*shader);

	if(!LogShaderCompile(*shader))
		return PACE_ERR_FAILURE;

	return PACE_ERR_SUCCESS;
}
