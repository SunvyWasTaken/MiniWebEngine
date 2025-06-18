// Sunset inc.

#include "Mesh.h"
#include "glad/glad.h"

namespace Sunset
{
	void VertexObject::ReserveVertices(const size_t size)
	{
		vertices.reserve(size);
	}

	void VertexObject::ReserveIndices(const size_t size)
	{
		indices.reserve(size);
	}

	Mesh::Mesh(const VertexObject& data)
		: VAO(0)
		, VBO(0)
		, EBO(0)
		, m_IndicesSize(data.indices.size())
	{
		glCreateVertexArrays(1, &VAO);
		glCreateBuffers(1, &VBO);
		glCreateBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(Vertex), data.vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(uint32_t), data.indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

		glBindVertexArray(0);
	}

	Mesh::~Mesh()
	{
		glDeleteBuffers(1, &EBO);
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
	}

	void Mesh::Draw() const
	{
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, m_IndicesSize, GL_UNSIGNED_INT, 0);
	}
}
