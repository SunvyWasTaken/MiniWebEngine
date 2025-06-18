// Sunset inc.

#pragma once

#include "TextureManager.h"

namespace Sunset
{
	class Mesh;
	class Material;

	class Drawable
	{
	public:
		Drawable(const std::shared_ptr<Mesh>& mesh = nullptr, const std::shared_ptr<Material>& material = nullptr);
		~Drawable();
		void Draw() const;
		void Bind(const glm::mat4& model);
	private:
		std::shared_ptr<Mesh> m_Mesh;
		std::shared_ptr<Material> m_Material;
	};
}
