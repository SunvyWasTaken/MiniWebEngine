// Sunset inc.

#pragma once

namespace Sunset
{
	struct TextCharacter
	{
		uint32_t textureId;
		glm::ivec2 size;
		glm::ivec2 bearing;
		uint32_t advance;
	};

	using TextCharacters = std::map<char, TextCharacter>;
}
