// Sunset inc.

#include "Material.h"
#include "Shaders.h"
#include "TextureBase.h"

#include "glad/glad.h"

namespace Sunset
{
	Material::Material(const std::shared_ptr<Shader>& shader, const AnyTexture& texture)
		: m_Shader(shader)
	{
		AddTexture(texture);
	}

	Material::~Material()
	{

	}

	void Material::Use() const
	{
		if (m_Shader)
		{
			m_Shader->Use();
		}
		int units[3] = { 0, 1, 2 };
		for (int slot = 0; auto& texture : m_Textures)
		{
			if (texture)
			{
				glActiveTexture(GL_TEXTURE0 + slot);
				m_Shader->SetUniformInt1v("textures", m_Textures.size(), units);
				texture.Use();
				++slot;
			}
		}
	}

	void Material::Bind(const glm::mat4& model)
	{
		if (!m_Shader)
			ENGINE_LOG_ERROR("There is no Shader attrib")

		m_Shader->Use();
		m_Shader->SetUniformMat4("model", model);
	}

	void Material::AddTexture(const AnyTexture& texture)
	{
		m_Textures.emplace_back(texture);
	}

}
