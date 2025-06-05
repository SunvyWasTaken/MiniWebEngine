// Sunset inc.

#include "Components/RenderComponent.h"
#include "Shaders.h"
#include "VertexObject.h"

namespace Sunset
{
	RenderComponent::RenderComponent(VertexObject* mesh)
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
