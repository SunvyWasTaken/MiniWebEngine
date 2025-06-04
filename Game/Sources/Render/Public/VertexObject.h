// Sunset inc.

#pragma once

namespace Sunset
{
	class VertexObject
	{
	public:
		explicit VertexObject(const std::vector<glm::vec3>& data);
		~VertexObject();

		void Draw();
	private:
		uint32_t VAO;
		uint32_t VBO;
		uint32_t EBO;
	};
}
