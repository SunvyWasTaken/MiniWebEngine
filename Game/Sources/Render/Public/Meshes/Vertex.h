// Sunset inc.

#pragma once

namespace Sunset
{
	struct StaticVertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoord;
	};

	struct SkeletalVertex : public StaticVertex
	{
		int boneIDs[4] = {-1, -1, -1, -1};
		float weights[4] = {0, 0, 0, 0};
	};

	template <typename VertexType>
	struct MeshData
	{
		using Type = VertexType;

		std::vector<VertexType> vertices;
		std::vector<uint32_t> indices;

		void PushVertice(const VertexType& vertex) 
		{
			vertices.emplace_back(vertex);
		}

		void PushIndice(const uint32_t indice)
		{
			indices.emplace_back(indice);
		}

		void Clear()
		{
			vertices.clear();
			indices.clear();
		}

		void ReserveVertices(const size_t size)
		{
			vertices.reserve(size);
		}

		void ReserveIndices(const size_t size)
		{
			indices.reserve(size);
		}
	};

	using StaticMeshData = MeshData<StaticVertex>;
	using SkeletalMeshData = MeshData<SkeletalVertex>;
}
