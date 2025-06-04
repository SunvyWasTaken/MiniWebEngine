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

	glm::f64vec2 Inputs::GetMouseScreenPosition()
	{
		glm::f64vec2 loc{0.0};
		glfwGetCursorPos((*(Engine::Get()->GetWindow()))(), &loc.x, &loc.y);
		return loc;
	}



	
}
