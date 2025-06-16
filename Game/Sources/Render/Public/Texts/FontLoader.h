// Sunset inc.

#pragma once

namespace Sunset
{
	class Shader;
	struct FontLoader
	{
		static void LoadFont(const std::string& path);

		static void RenderText(const std::shared_ptr<Shader>& shader, const std::string& text, float x, float y, float scale, const glm::vec3& color);
	};
}
