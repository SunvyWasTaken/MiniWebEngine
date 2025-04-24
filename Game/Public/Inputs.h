#pragma once

#include <glm/glm.hpp>

namespace Sunset
{
	struct Inputs
	{
		static bool IsKeyPressed(const int keyValue);

		static bool IsMouseButton(const int keyValue);

		static glm::vec2 GetMouseScreenPosition();
	};
}