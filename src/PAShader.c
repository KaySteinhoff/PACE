#include <PACE.h>

#define INVALID_UNIFORM_LOCATION 0xffffffff

void EnableShader(PAShader *shader)
{
	glUseProgram(shader->ID);
}

PAShader* LoadShaderFromSource(char *vertexSource, char *fragmentSource)
{
	FILE *source = fopen(vertexSource, "r");

	if(!source)
		return NULL;

	fseek(source, 0, SEEK_END);
	long fsize = ftell(source);
	fseek(source, 0, SEEK_SET);

	char *vertSource = malloc(fsize+1);
	if(!vertSource)
	{
		fclose(source);
		return NULL;
	}
	fread(vertSource, 1, fsize, source);

	fclose(source);
	vertSource[fsize] = '\0';

	source = fopen(fragmentSource, "r");
	if(!source)
		return NULL;

	fseek(source, 0, SEEK_END);
	fsize = ftell(source);
	fseek(source, 0, SEEK_SET);

	char *fragSource = malloc(fsize+1);
	if(!fragSource)
	{
		free(vertSource);
		fclose(source);
		return NULL;
	}
	fread(fragSource, fsize, 1, source);

	fclose(source);
	fragSource[fsize] = '\0';

	return CompileShader((const char*)vertSource, (const char*)fragSource);
}

int LogShaderCompile(GLuint shaderID)
{
	int success;
	char log[512];
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

	glGetShaderInfoLog(shaderID, 512, NULL, log);
	if(!success)
		printf("ERROR: SESHADER_COMPILATION_FAILED\nShader compilation log: %s\n", log);

	return success;
}

int SetInt(PAShader *shader, const char *name, int value)
{
	GLint location = glGetUniformLocation(shader->ID, name);

	if(location == INVALID_UNIFORM_LOCATION)
		return 0;

	glUniform1i(location, value);
	return 1;
}

int SetFloat(PAShader *shader, const char *name, float value)
{
	GLint location = glGetUniformLocation(shader->ID, name);

	if(location == INVALID_UNIFORM_LOCATION)
		return 0;

	glUniform1f(location, value);
	return 1;
}

PAShader* CompileShader(const char *vertexShader, const char *fragmentShader)
{
	PAShader *shader = malloc(sizeof(PAShader));

	if(!shader)
		return NULL;

	glGenBuffers(1, &shader->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, shader->vbo);

	shader->vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shader->vertexShader, 1, &vertexShader, NULL);
	glCompileShader(shader->vertexShader);

	if(!LogShaderCompile(shader->vertexShader))
	{
		printf("vertex shader failed\n");
Abort_Shader_Comp:
		free(shader);
		return NULL;
	}

	shader->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader->fragmentShader, 1, &fragmentShader, NULL);
	glCompileShader(shader->fragmentShader);

	if(!LogShaderCompile(shader->fragmentShader))
	{
		printf("fragment shader failed\n");
		goto Abort_Shader_Comp;
	}

	shader->ID = glCreateProgram();
	glAttachShader(shader->ID, shader->vertexShader);
	glAttachShader(shader->ID, shader->fragmentShader);
	glLinkProgram(shader->ID);

	if(!LogShaderCompile(shader->ID))
	{
		printf("Linking failed\n");
		goto Abort_Shader_Comp;
	}

	glDeleteShader(shader->vertexShader);
	glDeleteShader(shader->fragmentShader);

	shader->modelLocation = glGetUniformLocation(shader->ID, "model");
	shader->viewLocation = glGetUniformLocation(shader->ID, "view");
	shader->perspectiveLocation = glGetUniformLocation(shader->ID, "perspective");

	if(shader->modelLocation == INVALID_UNIFORM_LOCATION)
		printf("Failed to find model matrix location\n");
	if(shader->viewLocation == INVALID_UNIFORM_LOCATION)
		printf("Failed to find view matrix location\n");
	if(shader->perspectiveLocation == INVALID_UNIFORM_LOCATION)
		printf("Failed to find perspective matrix location\n");

	shader->texture = NULL;
	return shader;
}
