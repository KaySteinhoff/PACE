#include <PACE.h>
#include <GLFW/glfw3.h>

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
	mesh->numVertices = numVertices;
	mesh->numFaces = mesh->numVertices/5;

	glBufferData(GL_ARRAY_BUFFER, vertSize, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	return 1;
}

void DrawMesh(PAMesh *mesh)
{
	//Use mesh shader
	glUseProgram(mesh->shader->ID);
	//Transform current mesh
	mat4x4_apply_transform(mesh->transform.transformMatrix, &mesh->transform);
	glUniformMatrix4fv(mesh->shader->modelLocation, 1, GL_FALSE, (const GLfloat*)mesh->transform.transformMatrix);

	//Activate Texture of current mesh
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->shader->texture->textureID);
	//Use vertex array of current mesh
	glBindVertexArray(mesh->vao);
	glDrawArrays(GL_TRIANGLES, 0, mesh->numFaces);

	glBindVertexArray(0);
}

void PurgePAMesh(PAMesh *mesh)
{
	free(mesh->vertices);
	free(mesh->normals);
	free(mesh->uvs);

	free(mesh);
}
