// Sunset inc.

#include "Meshes/SkeletalMesh.h"

#include "glad/glad.h"

namespace Sunset
{
	SkeletalMesh::SkeletalMesh()
	{

	}

	SkeletalMesh::~SkeletalMesh()
	{

	}

	SkeletalMesh::SkeletalMesh(SkeletalMesh&& other) noexcept
		: Mesh(std::move(other))
	{
		m_Skeletal = std::move(other.m_Skeletal);
		other.m_Skeletal = {};
	}

	SkeletalMesh& SkeletalMesh::operator=(SkeletalMesh&& other) noexcept
	{
		if (this != &other)
		{
			Mesh::operator=(std::move(other));
			m_Skeletal = std::exchange(other.m_Skeletal, {});
			other.m_Skeletal = {};
		}
		return *this;
	}

	void SkeletalMesh::Update(float deltatime)
	{
		m_Skeletal.Update(deltatime);
		m_Skeletal.UpdateGPU();
	}

	void SkeletalMesh::AddSubMesh(const SkeletalMeshData& data)
	{
		uint32_t VAO, VBO, EBO;
		uint32_t size = data.indices.size();

		glCreateVertexArrays(1, &VAO);
		glCreateBuffers(1, &VBO);
		glCreateBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(typename SkeletalMeshData::Type), data.vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(uint32_t), data.indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(typename SkeletalMeshData::Type), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(typename SkeletalMeshData::Type), (void*)offsetof(typename SkeletalMeshData::Type, normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(typename SkeletalMeshData::Type), (void*)offsetof(typename SkeletalMeshData::Type, texCoord));

		glEnableVertexAttribArray(3);
		glVertexAttribIPointer(3, 4, GL_INT, sizeof(typename SkeletalMeshData::Type), (void*)offsetof(typename SkeletalMeshData::Type, boneIDs));

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(typename SkeletalMeshData::Type), (void*)offsetof(typename SkeletalMeshData::Type, weights));

		glBindVertexArray(0);

		m_SubMeshes.emplace_back(VAO, VBO, EBO, size);
	}
}
