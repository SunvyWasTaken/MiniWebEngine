// Sunset inc.

#pragma once

namespace Sunset
{
	struct AnyTexture;

	struct TextureLoader
	{
		static AnyTexture Load(const std::string& file);
		static AnyTexture Load(const std::string& name, std::array<std::string_view, 6>& files);
	};
}
