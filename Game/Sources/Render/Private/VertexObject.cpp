// Sunset inc.

#include "VertexObject.h"
#include "glad/glad.h"

namespace Sunset
{
	VertexObject::VertexObject(const std::vector<glm::vec3>& data)
		: VAO(0)
		, VBO(0)
		, EBO(0)
	{
		
	}

	VertexObject::~VertexObject()
	{
	}

	void VertexObject::Draw()
	{
		glDrawArrays()
	}
}
