// Sunset inc.

#include "Application.h"
#include "Camera.h"
#include "Inputs.h"
#include "Log.h"
#include "OpenGLWindow.h"
#include "Scene.h"
#include "SceneManager.h"


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
	Sunset::Engine* m_Engine = nullptr;

	bool bIsAppOpen = false;

	std::unique_ptr<Sunset::OpenGLRender> m_Render = nullptr;

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

	Engine* Engine::Get()
	{
		if (!m_Engine)
				m_Engine = new Sunset::Engine();

		return m_Engine;
	}

	void Engine::Destroy()
	{
		delete m_Engine;
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

			if (Inputs::IsKey(70))
			{
				bIsAppOpen = false;
			}

			if (Inputs::IsKey(68))
			{
				glm::vec3 dir = Deltatime * cam.GetCameraForwardVector();
				cam.AddPosition(dir);
			}

			m_SceneManager->Update(Deltatime);

			m_Render->Begin(cam);



			m_Render->End();
		}
	}

	OpenGLRender* Engine::GetWindow() const
	{
		return m_Render.get();
	}
}
