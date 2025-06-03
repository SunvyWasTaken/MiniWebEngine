// Sunset inc.

#include "Application.h"
#include "BasicRender.h"
#include "Log.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Camera.h"

#include "OpenGL/OpenGLWindow.h"

struct Menu : public Sunset::Scene
{
	Menu() : Scene()
	{
	}
	~Menu()
	{
	}

	virtual void Update(const float deltatime) override
	{
		Scene::Update(deltatime);
	}
};

struct Game : public Sunset::Scene
{
	Game() : Scene()
	{
	}

	~Game()
	{
	}

	virtual void Update(const float deltatime) override
	{
		Scene::Update(deltatime);
	}
};

namespace
{
	std::unique_ptr<Sunset::BasicRender> m_Render = nullptr;

	bool bIsAppOpen = false;

	std::unique_ptr<Sunset::SceneManager<Menu, Game>> m_SceneManager = nullptr;
}

namespace Sunset
{
	Engine::Engine()
	{
		bIsAppOpen = true;

		Log::Init();

		m_Render = std::make_unique<OpenGLRender>();
		m_SceneManager = std::make_unique<Sunset::SceneManager<Menu, Game>>();
	}

	Engine::~Engine()
	{
		m_SceneManager.reset();
		m_Render.reset();
		ENGINE_LOG_TRACE("Engine Destroyed")
	}

	void Engine::Run()
	{
		ENGINE_LOG_TRACE("Start Run")
		std::chrono::steady_clock::time_point _past = std::chrono::steady_clock::now();

		Camera cam;
		while (bIsAppOpen)
		{
			std::chrono::steady_clock::time_point _now = std::chrono::steady_clock::now();
			std::chrono::duration<float> delta = _now - _past;
			_past = _now;
			float Deltatime = delta.count();

			if (static_cast<OpenGLRender*>(m_Render.get())->IsKeyPressed(70))
			{
				bIsAppOpen = false;
			}

			if (static_cast<OpenGLRender*>(m_Render.get())->IsKeyPressed(87))
			{
				glm::vec3 dir = Deltatime * cam.GetCameraForwardVector();
				cam.AddPosition(dir);
			}

			m_SceneManager->Update(Deltatime);

			m_Render->Begin(cam);



			m_Render->End();
		}
	}
}
