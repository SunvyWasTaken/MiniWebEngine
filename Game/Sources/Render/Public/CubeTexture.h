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

		bool Load(const std::array<std::string_view, 6>& files);

		void Use() const;
	};
}
