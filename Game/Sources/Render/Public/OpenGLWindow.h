// Sunset inc.

class GLFWwindow;

namespace Sunset
{
	class Camera;

	class OpenGLRender
	{
	public:
		OpenGLRender();

		virtual ~OpenGLRender();

		virtual void Begin(const Camera& camera);

		virtual void End();

		GLFWwindow* operator()() const { return m_Window; }

	private:

		void CreateUniformBufferObject();

	private:

		GLFWwindow* m_Window;

		uint32_t m_Ubo;
	};
}
