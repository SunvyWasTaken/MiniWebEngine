// Sunset inc.

#include "Drawable.h"
#include "Material.h"
#include "Meshes/StaticMesh.h"
#include "Meshes/SkeletalMesh.h"

namespace Sunset
{
	Drawable::Drawable(const std::shared_ptr<Meshes>& mesh, const std::shared_ptr<Material>& material)
		: m_Mesh(mesh)
		, m_Material(material)
	{
		ENGINE_LOG_INFO("Drawable create")
	}

	Drawable::~Drawable()
	{
	}

	void Drawable::Update(float deltatime)
	{
		if (m_Mesh)
		{
			std::visit(
				[&](auto&& mesh)
				{
					if constexpr (requires{ mesh.Update(deltatime); })
					{
						mesh.Update(deltatime);
					}
				}, *m_Mesh);
		}
	}

	void Drawable::Draw() const
	{
		if (m_Material)
			m_Material->Use();

		if (m_Mesh)
		{
			std::visit(
				[&](auto&& mesh)
				{
					mesh.Draw();
				}, *m_Mesh);
		}
	}

	void Drawable::Bind(const glm::mat4& model)
	{
		if (m_Material)
			m_Material->Bind(model);
	}
}
