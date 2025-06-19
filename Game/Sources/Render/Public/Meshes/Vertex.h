// Sunset inc.

#pragma once

namespace Sunset
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoord;
	};

	struct VertexSkeletal : public Vertex
	{
		int boneIDs[4] = {0};
		int weights[4] = {0};
	};

	template <typename VertexType>
	struct VertexObject
	{
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

	using StaticVertices = VertexObject<Vertex>;
	using SkeletalVertices = VertexObject<VertexSkeletal>;

	using VerticeType = std::variant<StaticVertices, SkeletalVertices>;
}
