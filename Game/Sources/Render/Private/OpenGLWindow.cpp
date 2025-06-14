// Sunset inc.

#include "OpenGLWindow.h"
#include "Camera.h"

#include "glad/glad.h"
#include "glfw/glfw3.h"

#include "glm/gtc/type_ptr.hpp"

namespace Sunset
{
	OpenGLRender::OpenGLRender()
		: m_Window(nullptr)
		, m_Ubo(0)
	{
		ENGINE_LOG_TRACE("Window init")

		glfwInit();
		m_Window = glfwCreateWindow(1280, 720, "TinyEngine", nullptr, nullptr);

		glfwMakeContextCurrent(m_Window);
		glfwSwapInterval(1);
		glfwSetCursorPos(m_Window, 0, 0);
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			glfwTerminate();
			ENGINE_LOG_ERROR("Glad loader failed!")
			assert(false);
		}

		glViewport(0, 0, 1280, 720);
		glEnable(GL_DEPTH_TEST);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		CreateUniformBufferObject();
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

		glBindBuffer(GL_UNIFORM_BUFFER, m_Ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camera.GetProjection()));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera.GetView()));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void OpenGLRender::End()
	{
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}

	bool OpenGLRender::IsOpen() const
	{
		return !glfwWindowShouldClose(m_Window);
	}

	void OpenGLRender::CreateUniformBufferObject()
	{
		ENGINE_LOG_TRACE("Create uniform buffer for the camera.")
		glGenBuffers(1, &m_Ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, m_Ubo);
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_Ubo);
	}
}
