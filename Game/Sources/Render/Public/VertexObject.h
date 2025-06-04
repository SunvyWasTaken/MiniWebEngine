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
	};

	class VertexObject
	{
	public:
		explicit VertexObject(const Object& data);
		~VertexObject();

		void Draw();
	private:
		uint32_t VAO;
		uint32_t VBO;
		uint32_t EBO;

		size_t m_IndicesSize;
	};
}
