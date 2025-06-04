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

		GLFWwindow* operator()() { return m_Window; }

	private:

		GLFWwindow* m_Window;
	};
}
