// Sunset inc.

#pragma once

namespace Sunset
{
	class VertexObject;
	class Shader;

	class RenderComponent
	{
	public:
		explicit RenderComponent(VertexObject* mesh);
		~RenderComponent();

		void operator()(Shader* shader, const glm::mat4& model) const;

	private:
		VertexObject* m_Mesh;
	};
}
