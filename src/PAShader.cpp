#include <cppPACE.h>

#define INVALID_UNIFORM_LOCATION 0xffffffff

PAShader* PAShader::LoadShaderFromSource(char *vertexPath, char *fragmentPath)
{
	FILE *source = fopen(vertexPath, "r");

	if(!source)
		return NULL;

	fseek(source, 0, SEEK_END);
	long fsize = ftell(source);
	fseek(source, 0, SEEK_SET);

	char *vertexShader = (char*)malloc(fsize+1);
	if(!vertexShader)
	{
		fclose(source);
		return NULL;
	}
	fread(vertexShader, 1, fsize, source);
	fclose(source);
	vertexShader[fsize] = '\0';

	source = fopen(fragmentPath, "r");
	if(!source)
	{
		free(vertexShader);
		fclose(source);
		return NULL;
	}

	fseek(source, 0, SEEK_END);
	fsize = ftell(source);
	fseek(source, 0, SEEK_SET);

	char *fragmentShader = (char*)malloc(fsize+1);
	if(!fragmentShader)
	{
		free(vertexShader);
		fclose(source);
		return NULL;
	}
	fread(fragmentShader, 1, fsize, source);
	fclose(source);
	fragmentShader[fsize] = '\0';

	return PAShader::CompileShader((const char*)vertexShader, (const char*)fragmentShader);
}

int LogShaderCompileStatus(GLuint shaderID)
{
	int success;
	char log[512];
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

	glGetShaderInfoLog(shaderID, 512, NULL, log);
	if(!success)
		printf("ERROR: PAShader compilation failed:\n%s\n", log);

	return success;
}

PAShader* PAShader::CompileShader(const char *vertexShader, const char *fragmentShader)
{
	PAShader *shader = (PAShader*)malloc(sizeof(PAShader));

	if(!shader)
		return NULL;

	glGenBuffers(1, &shader->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, shader->vbo);

	shader->vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shader->vertexShader, 1, &vertexShader, NULL);
	glCompileShader(shader->vertexShader);

	if(!LogShaderCompileStatus(shader->vertexShader))
	{
		printf("Vertex shader compilation\n");
		free(shader);
		return NULL;
	}

	shader->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader->fragmentShader, 1, &fragmentShader, NULL);
	glCompileShader(shader->fragmentShader);

	if(!LogShaderCompileStatus(shader->fragmentShader))
	{
		printf("Fragment shader compilation\n");
		glDeleteShader(shader->vertexShader);
		free(shader);
		return NULL;
	}

	shader->ID = glCreateProgram();
	glAttachShader(shader->ID, shader->vertexShader);
	glAttachShader(shader->ID, shader->fragmentShader);
	glLinkProgram(shader->ID);

	if(!LogShaderCompileStatus(shader->ID))
	{
		printf("Shader program couldn't be linked!\n");
		glDeleteShader(shader->vertexShader);
		glDeleteShader(shader->fragmentShader);
		free(shader);
		return NULL;
	}

	glDeleteShader(shader->vertexShader);
	glDeleteShader(shader->fragmentShader);

	shader->modelLocation = glGetUniformLocation(shader->ID, "model");
	shader->viewLocation = glGetUniformLocation(shader->ID, "view");
	shader->perspectiveLocation = glGetUniformLocation(shader->ID, "perspective");

	if(shader->modelLocation == INVALID_UNIFORM_LOCATION || shader->viewLocation == INVALID_UNIFORM_LOCATION || shader->perspectiveLocation == INVALID_UNIFORM_LOCATION)
		printf("Failed to find uniform location!\n");

	shader->texture = NULL;

	return shader;
}

void PAShader::Enable()
{
	glUseProgram(ID);
}

int PAShader::SetInt(const char *name, int value)
{
	GLint location = glGetUniformLocation(ID, name);

	if(location == INVALID_UNIFORM_LOCATION)
		return 0;

	glUniform1i(location, value);
	return 1;
}

int PAShader::SetFloat(const char *name, float value)
{
	GLint location = glGetUniformLocation(ID, name);

	if(location == INVALID_UNIFORM_LOCATION)
		return 0;

	glUniform1f(location, value);
	return 1;
}
