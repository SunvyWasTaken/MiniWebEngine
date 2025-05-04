#include "Inputs.h"
#include "Render.h"

#include <glfw/glfw3.h>

namespace
{
	std::unordered_map<int, bool> keyPressed;
	std::unordered_map<int, bool> mousePressed;
}

namespace Sunset
{
	bool Inputs::IsKey(const int keyValue, const State::Type& type)
	{
		int state = glfwGetKey(Render::Get(), keyValue);

		return std::visit(overload{
		[&](const State::Hold&)
		{
			return state == GLFW_PRESS;
		},
		[&](const State::Pressed&)
		{
			if (!keyPressed.contains(keyValue))
				keyPressed.insert({keyValue, false});

			bool wasPressed = keyPressed.at(keyValue);
			keyPressed.at(keyValue) = (state == GLFW_PRESS);

			return (state == GLFW_PRESS && !wasPressed);
		} }, type);
	}

	bool Inputs::IsKeyReleased(const int keyValue)
	{
		int state = glfwGetKey(Render::Get(), keyValue);
		return state == GLFW_RELEASE;
	}

	bool Inputs::IsMouseButton(const int keyValue, const State::Type& type)
	{
		int state = glfwGetMouseButton(Render::Get(), keyValue);

		return std::visit(overload{
		[&](const State::Hold&)
		{
			return state == GLFW_PRESS;
		},
		[&](const State::Pressed&)
		{
			if (!mousePressed.contains(keyValue))
				mousePressed.insert({keyValue, false});

			bool wasPressed = mousePressed.at(keyValue);
			mousePressed.at(keyValue) = (state == GLFW_PRESS);

			return (state == GLFW_PRESS && !wasPressed);
		} }, type);
	}

	bool Inputs::IsMouseButtonReleased(const int keyValue)
	{
		int state = glfwGetMouseButton(Render::Get(), keyValue);
		return state == GLFW_RELEASE;
	}

	glm::vec2 Inputs::GetMouseScreenPosition()
	{
		double x, y;
		glfwGetCursorPos(Render::Get(), &x, &y);
		return glm::vec2{ x, y };
	}
}

