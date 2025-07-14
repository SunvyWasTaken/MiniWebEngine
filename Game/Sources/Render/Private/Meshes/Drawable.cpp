// Sunset inc.

#include "Meshes/Drawable.h"
#include "Material.h"
#include "Shaders.h"

namespace Sunset
{
	Drawable::Drawable(const std::shared_ptr<Meshes>& mesh, const std::shared_ptr<Material>& material)
		: m_Mesh(mesh)
		, m_Material(material)
	{
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
		glm::mat4 tmp = model;

		//std::visit([&](auto&& mesh)
		//{
		//	float s = mesh.GetSize();
		//	tmp = glm::scale(model, {s, s, s});
		//}, *m_Mesh);

		if (m_Material)
			m_Material->Bind(tmp);
	}
}
