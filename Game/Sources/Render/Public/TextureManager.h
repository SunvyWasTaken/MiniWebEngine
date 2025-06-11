// Sunset inc.

#pragma once

namespace Sunset
{
	template <typename T>
	class TextureBase;

	using Texture2D = TextureBase<class Texture>;
	using CubeMapTexture = TextureBase<class CubeTexture>;

	class TextureManager
	{
	public:
		TextureManager();
		~TextureManager();
		std::shared_ptr<Texture2D> Load(const std::string& file);
	private:
		std::unordered_map<std::string, std::weak_ptr<Texture2D>> m_Textures;
	};
}
