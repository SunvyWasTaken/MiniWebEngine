// Sunset inc.

#include "Meshes/StaticMesh.h"
#include "Meshes/Vertex.h"

#include "glad/glad.h"

namespace Sunset
{
	StaticMesh::StaticMesh()
		: Mesh()
	{
	}

	StaticMesh::~StaticMesh()
	{

	}

	StaticMesh::StaticMesh(StaticMesh&& other) noexcept
		: Mesh(std::move(other))
	{
	}

	StaticMesh& StaticMesh::operator=(StaticMesh&& other) noexcept
	{
		if (this != &other)
		{
			Mesh::operator=(std::move(other));
		}
		return *this;
	}

	void StaticMesh::AddSubMesh(const StaticMeshData& data)
	{
		uint32_t VAO, VBO, EBO;

		size_t size = data.indices.size();

		glCreateVertexArrays(1, &VAO);
		glCreateBuffers(1, &VBO);
		glCreateBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(StaticVertex), data.vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(uint32_t), data.indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(StaticVertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(StaticVertex), (void*)offsetof(StaticVertex, normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(StaticVertex), (void*)offsetof(StaticVertex, texCoord));

		glBindVertexArray(0);

		m_SubMeshes.emplace_back(VAO, VBO, EBO, size);
	}
}
