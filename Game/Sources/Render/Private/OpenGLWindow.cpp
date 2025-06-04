// Sunset inc.

#include "OpenGLWindow.h"
#include "Camera.h"

#include "glad/glad.h"
#include "glfw/glfw3.h"

namespace Sunset
{
	OpenGLRender::OpenGLRender()
	{
		glfwInit();
		m_Window = glfwCreateWindow(1280, 720, "TinyEngine", nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSwapInterval(1);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			glfwTerminate();
			ENGINE_LOG_ERROR("Glad loader failed!")
			assert(false);
		}
		glViewport(0, 0,1280, 720);

		ENGINE_LOG_TRACE("Window init")
	}

	OpenGLRender::~OpenGLRender()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();

		ENGINE_LOG_TRACE("Window Destroy")
	}

	void OpenGLRender::Begin(const Camera& camera)
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRender::End()
	{
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}
}
