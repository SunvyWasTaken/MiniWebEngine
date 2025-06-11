// Sunset inc.

#pragma once

namespace Sunset
{
	class Drawable;
	class Shader;

	class RenderComponent
	{
	public:
		explicit RenderComponent(std::shared_ptr<Drawable>& mesh);
		~RenderComponent();

		void operator()(Shader* shader, const glm::mat4& model) const;

	private:
		std::shared_ptr<Drawable> m_Mesh;
	};
}
