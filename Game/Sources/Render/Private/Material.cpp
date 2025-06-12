// Sunset inc.

#include "Material.h"
#include "Shaders.h"
#include "TextureBase.h"

namespace Sunset
{
	Material::Material(const std::shared_ptr<Shader>& shader, const AnyTexture& texture)
		: m_Shader(shader)
		, m_Texture(texture)
	{

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
		if (m_Texture)
		{
			m_Texture.Use();
		}
	}

	void Material::Bind(const glm::mat4& model)
	{
		if (!m_Shader)
			ENGINE_LOG_ERROR("There is no Shader attrib")

		m_Shader->Use();
		m_Shader->SetUniformMat4("model", model);
	}

}
