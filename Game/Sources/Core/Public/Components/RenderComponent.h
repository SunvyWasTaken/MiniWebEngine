// Sunset inc.

#pragma once

#include "Components/BaseComponent.h"

namespace Sunset
{
	class Drawable;
	class Shader;

	class RenderComponent : public BaseComponent
	{
	public:
		explicit RenderComponent(std::shared_ptr<Drawable>& mesh);
		virtual ~RenderComponent();

		void operator()(const glm::mat4& model) const;

	private:
		std::shared_ptr<Drawable> m_Mesh;
	};
}
