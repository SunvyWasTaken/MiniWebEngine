// Sunset inc.

#pragma once

#include "Textures/TextureManager.h"
#include "Meshes/StaticMesh.h"
#include "Meshes/SkeletalMesh.h"

namespace Sunset
{
	class Material;

	using Meshes = std::variant<StaticMesh, SkeletalMesh>;

	class Drawable
	{
	public:
		Drawable(const std::shared_ptr<Meshes>& mesh = nullptr, const std::shared_ptr<Material>& material = nullptr);

		~Drawable();

		void Update(float deltatime);

		void Draw() const;

		void Bind(const glm::mat4& model);

	private:

		std::shared_ptr<Meshes> m_Mesh;

		std::shared_ptr<Material> m_Material;
	};
}
