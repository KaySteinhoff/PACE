#include <PACEErrorHandling.h>
#include <PACE.h>
#include <linmath.h>
#include <GLFW/glfw3.h>

extern PACE *instance;
extern PATexture defaultPACETexture;

unsigned int TYPE_TAG_PAMESH = 0;

IPADraw newMesh(PAMesh *mesh)
{
	if(!mesh)
		return (IPADraw){ 0 };

	return (IPADraw){
		.typeTag = TYPE_TAG_PAMESH,
		.visible = 1,
		.data = mesh
	};
}

void MeshDraw(void *raw_data, mat4x4 perspective)
{
	PAMesh *this = (PAMesh*)raw_data;

	glUseProgram(this->shader->ID);
	glBindBuffer(GL_ARRAY_BUFFER, this->shader->vbo);
	glBindVertexArray(this->vao);
	//Transform current mesh
	mat4x4_apply_transform(this->transform.transformMatrix, &this->transform);
	glUniformMatrix4fv(this->shader->modelLocation, 1, GL_FALSE, (const GLfloat*)this->transform.transformMatrix);
	glUniformMatrix4fv(this->shader->viewLocation, 1, GL_FALSE, (const GLfloat*)instance->currentCamera->transform.transformMatrix);
	glUniformMatrix4fv(this->shader->perspectiveLocation, 1, GL_FALSE, (const GLfloat*)perspective);

	//Activate Texture of current mesh
		glActiveTexture(GL_TEXTURE0);
	if(!this->shader->texture)
		glBindTexture(GL_TEXTURE_2D, defaultPACETexture.textureID);
	else
		glBindTexture(GL_TEXTURE_2D, this->shader->texture->textureID);
	//Use vertex array of current mesh
	glDrawArrays(GL_TRIANGLES, 0, this->numVertices);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

float* CalculateNormals(float *vertices, uint32_t numVertices)
{
	int newSize = (numVertices/5)*3+numVertices;

	float *result = malloc(sizeof(float)*newSize);
	if(!result)
		return NULL;

	//Initialize v1 and v2 as 0, 0, 0 as they will be pulled with the first iteration
	vec3 v1 = { 0 };
	vec3 v2 = { 0 };

	//Initialize normal as 0, 0, 0 as it will be calculated with the first iteration
	vec3 normal = { 0 };
	for(int i = 0; i < numVertices/5; ++i)
	{
		if(i*8+7 > newSize-1)
			return NULL;

		result[i*8+0] = vertices[i*5+0];
		result[i*8+1] = vertices[i*5+1];
		result[i*8+2] = vertices[i*5+2];

		//Calculate normal, if needed
		if(i % 3 == 0)
		{
			//Pull current vertices
			v1[0] = vertices[i*5+0];
			v1[1] = vertices[i*5+1];
			v1[2] = vertices[i*5+2];

			v2[0] = vertices[i*5+10];
			v2[1] = vertices[i*5+11];
			v2[2] = vertices[i*5+12];

			//Calculate normal
			vec3_mul_cross(normal, v1, v2);
		}
		result[i*8+3] = normal[0];
		result[i*8+4] = normal[1];
		result[i*8+5] = normal[2];

		result[i*8+6] = vertices[i*5+3];
		result[i*8+7] = vertices[i*5+4];
	}

	free(vertices);
	return result;
}

unsigned int CreatePAMesh(PAMesh *mesh, PAMaterial *material, float *vertices, uint32_t numVertices)
{
	if(!mesh || !material)
		return PACE_ERR_NULL_REFERENCE;

	DEFAULT_TRANSFORM(defaultT);
	mesh->transform = defaultT;
	mesh->shader = material;
	mesh->numVertices = numVertices/8;

	glGenVertexArrays(1, &mesh->vao);
	glBindBuffer(GL_ARRAY_BUFFER, material->vbo);
	glBindVertexArray(mesh->vao);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*numVertices, vertices, GL_STATIC_DRAW);

	GLuint posLocation = glGetAttribLocation(mesh->shader->ID, "aPos");
	GLuint normalLocation = glGetAttribLocation(mesh->shader->ID, "aNormal");
	GLuint texLocation = glGetAttribLocation(mesh->shader->ID, "aTexCoord");

	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
	glEnableVertexAttribArray(posLocation);
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(normalLocation);
	glVertexAttribPointer(texLocation, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
	glEnableVertexAttribArray(texLocation);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return PACE_ERR_SUCCESS;
}
