#pragma once

#include "Object.h"
#include "RenderObject.h"

#include <glm/glm.hpp>

#include <variant>
#include <vector>
#include <memory>

class GLFWwindow;

namespace Sunset
{
	using Vertices = std::vector<glm::vec2>;

	class Camera;

	class Render
	{
	public:
		Render();
		~Render();

		operator bool ();

		static GLFWwindow* Get();

		// Begin Render sys
		void Begin(const std::shared_ptr<Camera>& cam);

		void End();

		void RenderObj(TransformComponent& transform, RenderObject& object);
		// End Render sys

		void Close(const bool bShouldClose);

		int GetWidth();

		int GetHeight();

		double GetTime();
	};

}