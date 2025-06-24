// Sunset inc.

#include "Textures/TextureManager.h"
#include "Textures/TextureBase.h"
#include "Textures/Texture.h"
#include "Textures/CubeTexture.h"

namespace
{
	std::unordered_map<std::string, std::weak_ptr<Sunset::Texture2D>> m_Textures;
	std::unordered_map<std::string, std::weak_ptr<Sunset::CubeMapTexture>> m_CubeTextures;
}

namespace Sunset
{
	AnyTexture TextureLoader::Load(const std::string& file)
	{
		auto it = m_Textures.find(file);
		if (it != m_Textures.end())
		{
			if (std::shared_ptr<Texture2D> shared = it->second.lock())
			{
				return *shared;
			}
			else
			{
				m_Textures.erase(file);
			}
		}

		std::shared_ptr<Texture> texture = std::make_shared<Texture>();
		if (!texture->LoadFromFile(file))
		{
			ENGINE_LOG_FATAL("Something went wrong while loading the texture file");
		}

		m_Textures.insert_or_assign(file, texture);
		return *texture;
	}

	AnyTexture TextureLoader::Load(const std::string& name, std::array<std::string_view, 6>& files)
	{
		auto it = m_CubeTextures.find(name);
		if (it != m_CubeTextures.end())
		{
			if (std::shared_ptr<CubeMapTexture> shared = it->second.lock())
			{
				return *shared;
			}
			else
			{
				m_Textures.erase(name);
			}
		}

		std::shared_ptr<CubeMapTexture> texture = std::make_shared<CubeMapTexture>();
		if (!texture->Load(files))
		{
			ENGINE_LOG_FATAL("Something went wrong while loading the texture file");
		}

		m_CubeTextures.insert_or_assign(name, texture);
		return *texture;
	}

}
