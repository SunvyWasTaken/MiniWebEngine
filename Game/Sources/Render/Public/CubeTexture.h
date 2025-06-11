// Sunset inc.

#pragma once

#include "TextureBase.h"

namespace Sunset
{
	class CubeTexture : public TextureBase<CubeTexture>
	{
	public:
		CubeTexture() = default;
		virtual ~CubeTexture();

		bool Load(const std::vector<std::string>& files);

		void Use() const;
	};
}
