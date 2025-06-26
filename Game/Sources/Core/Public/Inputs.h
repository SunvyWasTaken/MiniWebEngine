// Sunset inc.

#pragma once

namespace Sunset
{
	struct Inputs
	{
		static bool IsKey(int key);
		static bool IsKey(const char key);
		static glm::f64vec2 GetMouseScreenPosition();
		static glm::vec2 MouseMovement();
	};
}
