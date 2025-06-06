// Sunset inc.

#include "TextureManager.h"
#include "TextureBase.h"
#include "Texture.h"

namespace Sunset
{

	TextureManager::TextureManager()
	{

	}

	TextureManager::~TextureManager()
	{

	}

	std::shared_ptr<Sunset::Texture2D> TextureManager::Load(const std::string& file)
	{
		auto it = m_Textures.find(file);
		if (it != m_Textures.end())
		{
			if (std::shared_ptr<Texture2D>&& shared = it->second.lock())
			{
				return shared;
			}
			else
			{
				m_Textures.erase(file);
			}
		}

		std::shared_ptr<Texture> texture = std::make_shared<Texture>();
		if (!texture->LoadFromFile(file))
		{
			ENGINE_LOG_ERROR("Something went wrong while loading the texture file");
			return nullptr;
		}

		m_Textures.insert_or_assign(file, texture);
		return texture;
	}

}
