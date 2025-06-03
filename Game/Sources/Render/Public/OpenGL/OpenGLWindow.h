// Sunset inc.

#include "BasicRender.h"

class GLFWwindow;

namespace Sunset
{
	class OpenGLRender : public BasicRender
	{
	public:
		OpenGLRender();

		virtual ~OpenGLRender();

		bool IsKeyPressed(int key);

		virtual void Begin(const Camera& camera) override;

		virtual void End() override;

	private:

		GLFWwindow* Window();
	};
}
