// Sunset inc.

#include "Material.h"
#include "Shaders.h"
#include "Textures/TextureBase.h"

#include "glad/glad.h"

namespace Sunset
{
	Material::Material(const std::shared_ptr<Shader>& shader, const AnyTexture& texture)
		: m_Shader(shader)
	{
		AddTexture(texture);
		ENGINE_LOG_TRACE("Material Create")
	}

	Material::~Material()
	{
		ENGINE_LOG_TRACE("Material Delete")
	}

	void Material::Bind(const glm::mat4& model)
	{
		if (!m_Shader)
		{
			ENGINE_LOG_FATAL("There is no Shader attrib")
		}

		if (m_Textures.empty())
			ENGINE_LOG_WARN("Material has no texture attached");

		m_Shader->Use();
		m_Shader->SetUniformMat4("model", model);

		std::vector<int> units;
		units.reserve(m_Textures.size());

		for (int slot = 0; auto & texture : m_Textures)
		{
			if (texture)
			{
				glActiveTexture(GL_TEXTURE0 + slot);
				texture.Use();
				units.emplace_back(slot);
				++slot;
			}
		}

		m_Shader->SetUniformInt1v("textures", m_Textures.size(), units.data());
	}

	void Material::AddTexture(const AnyTexture& texture)
	{
		m_Textures.emplace_back(texture);
	}

}
