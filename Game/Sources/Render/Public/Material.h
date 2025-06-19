// Sunset inc.

#pragma once

#include "Textures/TextureBase.h"

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
		void AddTexture(const AnyTexture& texture);
	private:
		std::shared_ptr<Shader> m_Shader;
		std::vector<AnyTexture> m_Textures;
	};
}
