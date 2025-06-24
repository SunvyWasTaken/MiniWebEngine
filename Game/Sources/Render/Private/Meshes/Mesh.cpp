// Sunset inc.

#include "Meshes/Mesh.h"

#include <glad/glad.h>

namespace Sunset
{

	SubMesh::SubMesh(uint32_t vao, uint32_t vbo, uint32_t ebo, size_t size)
		: VAO(vao)
		, VBO(vbo)
		, EBO(ebo)
		, m_IndicesSize(size)
	{

	}

	SubMesh::~SubMesh()
	{
		if (VAO) glDeleteVertexArrays(1, &VAO);
		if (VBO) glDeleteBuffers(1, &VBO);
		if (EBO) glDeleteBuffers(1, &EBO);
	}

	void SubMesh::Draw() const
	{
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, m_IndicesSize, GL_UNSIGNED_INT, 0);
	}

	SubMesh::SubMesh(SubMesh&& other) noexcept
	{
		VAO = std::exchange(other.VAO, 0);
		VBO = std::exchange(other.VBO, 0);
		EBO = std::exchange(other.EBO, 0);
		m_IndicesSize = std::exchange(other.m_IndicesSize, 0);
	}

	SubMesh& SubMesh::operator=(SubMesh&& other) noexcept
	{
		if (this != &other)
		{
			VAO = std::exchange(other.VAO, 0);
			VBO = std::exchange(other.VBO, 0);
			EBO = std::exchange(other.EBO, 0);
			m_IndicesSize = std::exchange(other.m_IndicesSize, 0);
		}
		return *this;
	}

	Mesh::Mesh()
		: m_SubMeshes()
		, m_ImportSize(1.f)
	{
	}

	Mesh::~Mesh()
	{
	}

	void Mesh::Draw() const
	{
		for (auto& subMesh : m_SubMeshes)
		{
			subMesh.Draw();
		}
	}

	Mesh::Mesh(Mesh&& other) noexcept
		: m_SubMeshes(std::exchange(other.m_SubMeshes, {}))
		, m_ImportSize(std::exchange(other.m_ImportSize, 0.f))
	{
	}

	Mesh& Mesh::operator=(Mesh&& other) noexcept
	{
		if (this != &other)
		{
			m_SubMeshes = std::exchange(other.m_SubMeshes, {});
			m_ImportSize = std::exchange(other.m_ImportSize, 0.f);
		}
		return *this;
	}
}
