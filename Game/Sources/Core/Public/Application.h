// Sunset inc.

#pragma once

namespace Sunset
{
	class Engine
	{
	private:
		Engine();

		~Engine();

	public:
		static Engine* Get();

		static void Destroy();

	public:
		void Run();

		class OpenGLRender* GetWindow() const;

		// Renderer
		// PhysicsSys
		// InputSys
		// RessourceManager
	};
}
