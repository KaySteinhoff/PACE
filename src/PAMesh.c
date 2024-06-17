#include <PACE.h>
#include <linmath.h>
#include <GLFW/glfw3.h>

int SetPAMeshVertices(PAMesh *mesh, float *vertices, uint32_t numVertices);

IPADraw newMesh(PAShader *shader, float *vertices, uint32_t numVertices)
{
	PAMesh *mesh = malloc(sizeof(PAMesh));
	if(!mesh)
		return (IPADraw){ 0 };

	DEFAULT_TRANSFORM(defaultT);
	mesh->transform = defaultT;

	glGenVertexArrays(1, &mesh->vao);
	mesh->shader = shader;
	SetPAMeshVertices(mesh, vertices, numVertices);

	return (IPADraw){
		.typeTag = TYPE_TAG_PAMESH,
		.data = mesh
	};
}

void MeshDraw(void *raw_data, mat4x4 perspective)
{
	PAMesh *this = (PAMesh*)raw_data;

	glUseProgram(this->shader->ID);
	//Transform current mesh
	mat4x4_apply_transform(this->transform.transformMatrix, &this->transform);
	glUniformMatrix4fv(this->shader->modelLocation, 1, GL_FALSE, (const GLfloat*)this->transform.transformMatrix);

	//Activate Texture of current mesh
	if(this->shader->texture->textureID)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->shader->texture->textureID);
	}
	//Use vertex array of current mesh
	glBindVertexArray(this->vao);
	glDrawArrays(GL_TRIANGLES, 0, this->numVertices);

	glBindVertexArray(0);

	glUniformMatrix4fv(this->shader->viewLocation, 1, GL_FALSE, (const GLfloat*)GetInstance()->currentCamera->transform.transformMatrix);
/*	if(GetInstance()->currentCamera->viewMode == PAProjection)
		glUniformMatrix4fv(this->shader->perspectiveLocation, 1, GL_FALSE, (const GLfloat*)GetInstance()->currentCamera->projectionMatrix);
	else
		glUniformMatrix4fv(this->shader->perspectiveLocation, 1, GL_FALSE, (const GLfloat*)GetInstance()->currentCamera->orthoMatrix);
*/	glUniformMatrix4fv(this->shader->perspectiveLocation, 1, GL_FALSE, (const GLfloat*)perspective);

}

float* CalculateNormals(float *vertices, uint32_t numVertices)
{
	int newSize = (numVertices/5)*3+numVertices;
	printf("old count: %d\nnew count: %d\n", numVertices, newSize);

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

PAMesh* CreateMesh()
{
	PAMesh *mesh = malloc(sizeof(PAMesh));

	if(!mesh)
		return NULL;

	DEFAULT_TRANSFORM(defaultT);
	mesh->transform = defaultT;

	glGenVertexArrays(1, &mesh->vao);
	mesh->shader = NULL;

	return mesh;
}

int SetPAMeshVertices(PAMesh *mesh, float *vertices, uint32_t numVertices)
{
	glBindVertexArray(mesh->vao);

	size_t vertSize = sizeof(float)*numVertices;
	mesh->numVertices = numVertices/8;

	glBufferData(GL_ARRAY_BUFFER, vertSize, vertices, GL_STATIC_DRAW);

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
	return 1;
}

void PurgePAMesh(PAMesh *mesh)
{
	free(mesh);
}
