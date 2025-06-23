// Sunset inc.

#include "Meshes/Mesh.h"

#include <glad/glad.h>

namespace Sunset
{
	Mesh::Mesh()
	{
	}

	Mesh::~Mesh()
	{
	}

	void Mesh::Draw() const
	{
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, m_IndicesSize, GL_UNSIGNED_INT, 0);
	}

	Mesh::Mesh(Mesh&& other) noexcept
	{
		this->VAO = std::exchange(other.VAO, 0);
		this->VBO = std::exchange(other.VBO, 0);
		this->EBO = std::exchange(other.EBO, 0);
		this->m_IndicesSize = std::exchange(other.m_IndicesSize, 0);
		this->m_ImportSize = std::exchange(other.m_ImportSize, 0);
	}

	Mesh& Mesh::operator=(Mesh&& other) noexcept
	{
		if (this != &other) 
		{
			this->VAO = std::exchange(other.VAO, 0);
			this->VBO = std::exchange(other.VBO, 0);
			this->EBO = std::exchange(other.EBO, 0);
			this->m_IndicesSize = std::exchange(other.m_IndicesSize, 0);
			this->m_ImportSize = std::exchange(other.m_ImportSize, 0);
		}
		return *this;
	}

	SubMesh::SubMesh()
	{

	}

	SubMesh::~SubMesh()
	{

	}

	void SubMesh::Draw() const
	{

	}

	uint32_t SubMesh::SubMesh()
	{

	}

	uint32_t SubMesh::operator=()
	{

	}

}
