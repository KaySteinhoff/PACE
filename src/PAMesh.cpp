#include <cppPACE.h>

PAMesh::PAMesh()
{
	transform = PATransform();
	glGenVertexArrays(1, &vao);
	shader = NULL;
}

int PAMesh::SetVertices(float *vertices, uint32_t numVertices)
{
	if(!vertices || numVertices <= 0)
		return 0;

	glBindVertexArray(vao);

	size_t vertSize = numVertices*sizeof(float);
	this->numVertices = numVertices;
	numFaces = numVertices/5;

	glBufferData(GL_ARRAY_BUFFER, vertSize, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	return 1;
}

void PAMesh::Draw()
{
	transform.ApplyTransform(transform.transformMatrix);
	glUniformMatrix4fv(shader->modelLocation, 1, GL_FALSE, (const GLfloat*)transform.transformMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shader->texture->textureID);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, numFaces);

	glBindVertexArray(0);
}

PAMesh::~PAMesh()
{
	free(vertices);
	free(shader);
}
