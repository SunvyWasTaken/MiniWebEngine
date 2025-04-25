#include "Inputs.h"
#include "Render.h"

#include <glfw/glfw3.h>

namespace Sunset
{
	bool Inputs::IsKeyPressed(const int keyValue)
	{
		int state = glfwGetKey(Render::Get(), keyValue);
		return state == GLFW_PRESS;
	}

	bool Inputs::IsMouseButton(const int keyValue)
	{
		int state = glfwGetMouseButton(Render::Get(), keyValue);
		return state == GLFW_PRESS;
	}

	glm::vec2 Inputs::GetMouseScreenPosition()
	{
		double x, y;
		glfwGetCursorPos(Render::Get(), &x, &y);
		return glm::vec2{ x, y };
	}

}

