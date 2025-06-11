// Sunset inc.

#pragma once

#include "TextureManager.h"
#include "VertexObject.h"

namespace Sunset
{
	class Drawable
	{
	public:
		Drawable();
		~Drawable();
		void Draw() const;
	private:
		std::shared_ptr<VertexObject> m_Mesh;
		std::shared_ptr<Texture2D> m_Texture;
	};

	class CubeMap final
	{
	public:
		CubeMap();
		~CubeMap();
		void Draw() const;
		void Load(const std::vector<std::string>& textures);
	private:
		std::shared_ptr<CubeMapTexture> m_Texture;
	};
}
