#pragma once

#include "RenderObject.h"
#include "Components.h"

#ifndef __EMSCRIPTEN__
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#endif

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

		void RenderObj(const TransformComponent& transform, const RenderObject& object);
		// End Render sys

		void Close(const bool bShouldClose);

		static int GetWidth();

		static int GetHeight();

		double GetTime();
	};

}