#include "UTILITY.h"
#include "GL/glew.h"

#define BUFFER_OFFSET(i) ((char *)0 + (i))

GLuint _FullscreenQuadVAO = GL_NONE;
GLuint _FullscreenQuadVBO = GL_NONE;


void initFullscreenQuad()
{
	float VBO_DATA[] = 
	{
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,

		1.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
	};

	int vertexSize = 6 * 3 * sizeof(float);
	int texCoordSize = 6 * 2 * sizeof(float);

	glGenVertexArrays(1, &_FullscreenQuadVAO);
	glBindVertexArray(_FullscreenQuadVAO);

	glEnableVertexAttribArray(0); //verts
	glEnableVertexAttribArray(1); //UVs

	glGenBuffers(1, &_FullscreenQuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _FullscreenQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, vertexSize + texCoordSize, VBO_DATA, GL_STATIC_DRAW);

	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertexSize));

	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	glBindVertexArray(GL_NONE);
}

void drawFullscreenQuad()
{
	glBindVertexArray(_FullscreenQuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(GL_NONE);
}
