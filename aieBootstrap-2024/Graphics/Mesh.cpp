#include "Mesh.h"
#include <gl_core_4_4.h>

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
}

void Mesh::Initialise(unsigned int _vertexCount, const Vertex* _vertices, unsigned int _indexCount, unsigned int* _indices)
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

	// fill vertex buffer
	glBufferData(GL_ARRAY_BUFFER, _vertexCount * sizeof(Vertex), _vertices, GL_STATIC_DRAW);

	// enable first element as position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	
	// bind indices if there are any
	if (_indexCount != 0)
	{
		glGenBuffers(1, &m_ibo);

		// bind vertex buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

		// fill vetex buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexCount * sizeof(unsigned int), _indices, GL_STATIC_DRAW);

		m_triCount = _indexCount / 3;
	}
	else
		m_triCount = _vertexCount / 3;

	// unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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
	vertices[4].position = { 0.5f,  0.f, 0.5f, 1 };
	vertices[5].position = { 0.5f, 0.f, -0.5f, 1 };	
	
	// All nomrals point up
	vertices[0].normal = { 0,1,0,0 };
	vertices[1].normal = { 0,1,0,0 };
	vertices[2].normal = { 0,1,0,0 };
	
	vertices[3].normal = { 0,1,0,0 };
	vertices[4].normal = { 0,1,0,0 };
	vertices[5].normal = { 0,1,0,0 };

	vertices[0].texCoord = { 0,1 }; // Bottom Left;
	vertices[1].texCoord = { 1,1 }; // Bottom Right;
	vertices[2].texCoord = { 0,0 }; // Top Left;

	vertices[3].texCoord = { 0,0 }; // Top Left;
	vertices[4].texCoord = { 1,1 }; // Bottom Right;
	vertices[5].texCoord = { 1,0 }; // Top Right;



	// Fill vertex buffer
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// Set the first element to be a position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// Set the second elemen to be the normal
	glDisableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex),(void*)16);

	// Set the third element to be the texture coordinate
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)32);
	
	// Unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Set the tri count
	m_triCount = 2;
}

void Mesh::InitialiseFullScreenQuad()
{
	// check that the mesh is not initialized aleady
	assert(m_vao == 0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// generate bffers
	glGenBuffers(1, &m_vbo);
	glGenVertexArrays(1, &m_vao);

	// bind vertext array aka a mesh wrapper
	glBindVertexArray(m_vao);

	// bind vertext buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	// Define the 6 vertices for 2 triangles

	float vertices[] = {

	   -1, 1, // Left Top
	   -1,-1, // Left Bottom
		1, 1, // Right Top
	   
		-1,-1, // Left Bottom
		1,-1, // Right Bottom
		1, 1,  // Right top
	};
	
	// Fill vertex buffer
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices, GL_STATIC_DRAW);

	// Set the first element to be a position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8, 0);

	// Unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Set the Tri count
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
