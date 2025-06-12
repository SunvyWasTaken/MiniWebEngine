// Sunset inc.

#pragma once

#include "TextureBase.h"

namespace Sunset
{
	class Shader;

	class Material
	{
	public:
		Material(const std::shared_ptr<Shader>& shader, const AnyTexture& texture);
		~Material();
		void Use() const;
		void Bind(const glm::mat4& model);
	private:
		std::shared_ptr<Shader> m_Shader;
		AnyTexture m_Texture;
	};
}
