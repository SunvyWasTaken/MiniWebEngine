#pragma once

#include <glm/glm.hpp>

namespace Sunset
{
	struct Inputs
	{
		static bool IsKeyPressed(const int keyValue);

		static bool IsKeyReleased(const int keyValue);

		static bool IsMouseButtonPressed(const int keyValue);

		static bool IsMouseButtonReleased(const int keyValue);

		static glm::vec2 GetMouseScreenPosition();
	};
}