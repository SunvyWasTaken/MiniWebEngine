// Sunset inc.

#include "Components/RenderComponent.h"
#include "Shaders.h"
#include "Meshes/Drawable.h"

namespace Sunset
{
	RenderComponent::RenderComponent(std::shared_ptr<Drawable>& mesh)
		: BaseComponent()
		, m_Mesh(mesh)
	{
	}

	RenderComponent::~RenderComponent()
	{
	}

	void RenderComponent::operator()(const glm::mat4& model) const
	{
		m_Mesh->Bind(model);
		m_Mesh->Draw();
	}
}
