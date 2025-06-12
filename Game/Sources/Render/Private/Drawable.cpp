// Sunset inc.

#include "Drawable.h"
#include "Material.h"
#include "VertexObject.h"

namespace Sunset
{
	Drawable::Drawable(const std::shared_ptr<VertexObject>& mesh, const std::shared_ptr<Material>& material)
		: m_Mesh(mesh)
		, m_Material(material)
	{
	}

	Drawable::~Drawable()
	{
	}

	void Drawable::Draw() const
	{
		if (m_Material)
			m_Material->Use();

		if (m_Mesh)
			m_Mesh->Draw();
	}

	void Drawable::Bind(const glm::mat4& model)
	{
		if (m_Material)
			m_Material->Bind(model);
	}
}
