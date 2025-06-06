// Sunset inc.

#pragma once

namespace Sunset
{
	class VertexObject;
	class Shader;

	class RenderComponent
	{
	public:
		explicit RenderComponent(std::shared_ptr<VertexObject>& mesh);
		~RenderComponent();

		void operator()(Shader* shader, const glm::mat4& model) const;

	private:
		std::shared_ptr<VertexObject> m_Mesh;
	};
}
