// Sunset inc.

#pragma once

namespace Sunset
{
	class Engine
	{
	protected:
		Engine();

		virtual ~Engine();

	public:
		template <typename T>
		static Engine* Create()
		{
			if (!m_Engine)
				m_Engine = new T();

			return m_Engine;
		}

		static Engine* Get()
		{
			return m_Engine;
		}

		static void Destroy();

	public:

		void Run();

		class OpenGLRender* GetWindow() const;

		static class Scene* GetWorld();

	protected:

		virtual void LogicLoop(const float deltatime);

		class Camera& GetCam() const;

	private:

		static Engine* m_Engine;

		// Renderer
		// PhysicsSys
		// InputSys
		// RessourceManager
	};
}
