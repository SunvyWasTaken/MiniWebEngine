// Sunset inc.

#pragma once

#include "TextureBase.h"

namespace Sunset
{
	class Texture : public TextureBase<Texture>
	{
	public:
		Texture();
		virtual ~Texture();
		bool LoadFromFile(const std::string& file);
		void Use() const;
	};
}
