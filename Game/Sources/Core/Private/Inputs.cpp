// Sunset inc.

#include "Inputs.h"
#include "Application.h"
#include "OpenGLWindow.h"

#include "glfw/glfw3.h"

namespace Sunset
{
	bool Inputs::IsKey(int key)
	{
		return glfwGetKey((*(Engine::Get()->GetWindow()))(), key) == GLFW_PRESS;
	}

	bool Inputs::IsKey(const char key)
	{
		return IsKey(static_cast<int>(key));
	}

	glm::f64vec2 Inputs::GetMouseScreenPosition()
	{
		glm::f64vec2 loc{0.0};
		glfwGetCursorPos((*(Engine::Get()->GetWindow()))(), &loc.x, &loc.y);
		return loc;
	}

	glm::vec2 Inputs::MouseMovement()
	{
		static glm::f64vec2 mPos(0.f);
		glm::vec2 delta = GetMouseScreenPosition() - mPos;
		mPos = GetMouseScreenPosition();
		return delta;
	}



	
}
