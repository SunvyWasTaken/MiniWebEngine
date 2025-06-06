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

	struct Object
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		void PushVertice(const Vertex& vertex) { vertices.emplace_back(vertex); }
		void PushIndice(const uint32_t indice) { indices.emplace_back(indice); }

		void Clear() { vertices.clear(); indices.clear(); }
	};

	class VertexObject
	{
	public:
		explicit VertexObject(const Object& data);
		~VertexObject();

		void Draw() const;
	private:
		uint32_t VAO;
		uint32_t VBO;
		uint32_t EBO;

		size_t m_IndicesSize;
	};
}
