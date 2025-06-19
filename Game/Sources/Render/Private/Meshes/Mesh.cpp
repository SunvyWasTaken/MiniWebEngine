// Sunset inc.

#include "Meshes/Mesh.h"


namespace Sunset
{
	Mesh::Mesh()
	{
		ENGINE_LOG_INFO("mesh create")
	}

	Mesh::~Mesh()
	{
		ENGINE_LOG_INFO("mesh Destroy")
	}

	Mesh::Mesh(Mesh&& other) noexcept
	{
		this->VAO = std::exchange(other.VAO, 0);
		this->VBO = std::exchange(other.VBO, 0);
		this->EBO = std::exchange(other.EBO, 0);
		this->m_IndicesSize = std::exchange(other.m_IndicesSize, 0);
	}

	Mesh& Mesh::operator=(Mesh&& other) noexcept
	{
		if (this != &other) 
		{
			this->VAO = std::exchange(other.VAO, 0);
			this->VBO = std::exchange(other.VBO, 0);
			this->EBO = std::exchange(other.EBO, 0);
			this->m_IndicesSize = std::exchange(other.m_IndicesSize, 0);
		}
		return *this;
	}
}
