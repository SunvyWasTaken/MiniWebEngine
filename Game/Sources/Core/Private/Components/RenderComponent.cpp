// Sunset inc.

#include "Components/RenderComponent.h"
#include "Shaders.h"
#include "Drawable.h"

namespace Sunset
{
	RenderComponent::RenderComponent(std::shared_ptr<Drawable>& mesh)
		: m_Mesh(mesh)
	{
	}

	RenderComponent::~RenderComponent()
	{
	}

	void RenderComponent::operator()(Shader* shader, const glm::mat4& model) const
	{
		shader->SetUniformMat4("model", model);
		m_Mesh->Draw();
	}
}
