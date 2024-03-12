#include "Mesh.h"
#include <gl_core_4_4.h>

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
}

void Mesh::InitialiseQuad()
{
	// check that the mesh is not initialized aleady
	assert(m_vao == 0);

	// generate bffers
	glGenBuffers(1, &m_vbo);
	glGenVertexArrays(1, &m_vao);

	// bind vertext array aka a mesh wrapper
	glBindVertexArray(m_vao);

	// bind vertext buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vao);

	// Define the 6 vertices for 2 triangles

	Vertex vertices[6];
	vertices[0].position = { -0.5f, 0.f, 0.5f, 1 };
	vertices[1].position = { 0.5f, 0.f, 0.5f, 1 };
	vertices[2].position = { -0.5f, 0.f, -0.5f, 1 };

	vertices[3].position = { -0.5f, 0.f, -0.5f, 1 };
	vertices[4].position = { 0.5f, 0.f, 0.5f, 1 };
	vertices[5].position = { 0.5f, 0.f, -0.5f, 1 };	
	
	// Fill vertex buffer
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// Set the first element to be a position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// Unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Set the tri count
	m_triCount = 2;
}

void Mesh::Draw()
{
	glBindVertexArray(m_vao);

	if (m_ibo != 0)
		glDrawElements(GL_TRIANGLES, 3 * m_triCount, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, 3 * m_triCount);
}
