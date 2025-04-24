#include "Camera.h"
#include "Object.h"
#include "Render.h"

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

#include <glfw/glfw3.h>

#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace Sunset
{
	GLFWwindow* m_Window;

	glm::i32vec2 m_WindowSize{1280, 720};

	uint32_t VAO = 0;
	uint32_t VBO = 0;

	std::shared_ptr<Camera> camera;

	// Function call at the resize of the window
	void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		m_WindowSize = glm::vec2(width, height);
		glViewport(0, 0, width, height);
	}

	void InitWindow()
	{
		glfwInit();
#ifdef __EMSCRIPTEN__
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif // __EMSCRIPTEN__

		m_Window = glfwCreateWindow(m_WindowSize.x, m_WindowSize.y, "Game", NULL, NULL);
		if (m_Window == NULL)
		{
			glfwTerminate();
			assert(false);
		}
		glfwMakeContextCurrent(m_Window);

		glfwSetFramebufferSizeCallback(m_Window, &FrameBufferSizeCallback);

#ifdef __EMSCRIPTEN__
#else
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			glfwTerminate();
			assert(false);
		}
#endif //__EMSCRIPTEN__

		glViewport(0, 0, m_WindowSize.x, m_WindowSize.y);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	Render::Render()
	{
		InitWindow();
	}

	Render::~Render()
	{
		glfwTerminate();
	}

	Render::operator bool()
	{
		return glfwWindowShouldClose(m_Window);
	}

	GLFWwindow* Render::Get()
	{
		return m_Window;
	}

	void Render::Begin(const std::shared_ptr<Camera>& cam)
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera = cam;
	}

	void Render::End()
	{
		glUseProgram(0);
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}

	void Render::RenderObj(TransformComponent& transform, RenderObject& object)
	{
		object(transform, camera);
	}

	void Render::Close(const bool bShouldClose)
	{
		glfwSetWindowShouldClose(m_Window, bShouldClose);
	}

	int Render::GetWidth()
	{
		return m_WindowSize.x;
	}

	int Render::GetHeight()
	{
		return m_WindowSize.y;
	}

	double Render::GetTime()
	{
		return glfwGetTime();
	}

}

