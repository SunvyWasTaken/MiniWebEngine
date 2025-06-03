// Sunset inc.

#include "OpenGL/OpenGLWindow.h"
#include "Camera.h"

#include "glad/glad.h"
#include "glfw/glfw3.h"

namespace Sunset
{
	OpenGLRender::OpenGLRender()
	{
		glfwInit();
		m_Window = glfwCreateWindow(1280, 720, "TinyEngine", nullptr, nullptr);
		glfwMakeContextCurrent(Window());
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
		glfwDestroyWindow(Window());
		glfwTerminate();

		ENGINE_LOG_TRACE("Window Destroy")
	}

	bool OpenGLRender::IsKeyPressed(int key)
	{
		return glfwGetKey(Window(), key) == GLFW_PRESS;
	}

	void OpenGLRender::Begin(const Camera& camera)
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRender::End()
	{
		glfwSwapBuffers(Window());
		glfwPollEvents();
	}

	GLFWwindow* OpenGLRender::Window()
	{
		return static_cast<GLFWwindow*>(m_Window);
	}
}
