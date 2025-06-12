// Sunset inc.

#pragma once

#include "TextureManager.h"

namespace Sunset
{
	class VertexObject;
	class Material;

	class Drawable
	{
	public:
		Drawable(const std::shared_ptr<VertexObject>& mesh = nullptr, const std::shared_ptr<Material>& material = nullptr);
		~Drawable();
		void Draw() const;
		void Bind(const glm::mat4& model);
	private:
		std::shared_ptr<VertexObject> m_Mesh;
		std::shared_ptr<Material> m_Material;
	};
}
