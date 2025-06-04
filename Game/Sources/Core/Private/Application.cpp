// Sunset inc.

#include "Application.h"
#include "Camera.h"
#include "Entity.h"
#include "Inputs.h"
#include "Log.h"
#include "OpenGLWindow.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Shaders.h"
#include "VertexObject.h"
#include "Components/TransformComponent.h"
#include "Components/RenderComponent.h"

#include <random>


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

	Sunset::Object data = { {Sunset::Vertex	{{ 0.0, 0.5, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0} },
											{{-0.5, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0} },
											{{ 0.5, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0} }}
											, {0, 1, 2}};

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(-10, 10);

	Sunset::Entity SpawnEntity(Sunset::VertexObject* mesh)
	{
		if (!m_SceneManager)
		{
			ENGINE_LOG_ERROR("Something went wrong and u just create a bad Entity.")
			return Sunset::Entity{nullptr};
		}

		Sunset::Entity currentEntity = m_SceneManager->GetScene()->CreateEntity();
		float x = dist(gen);
		float y = dist(gen);
		x*=0.1;
		y*=0.1;

		currentEntity.AddComponent<Sunset::TransformComponent>(glm::vec3{x, y, 0});
		currentEntity.AddComponent<Sunset::RenderComponent>(mesh);

		return currentEntity;
	}
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
		
		VertexObject cube{data};
		Shader shade{"../../Game/Sources/Shaders/vShader.glsl", "../../Game/Sources/Shaders/fShader.glsl"};

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
				SpawnEntity(&cube);
			}

			m_SceneManager->Update(Deltatime);

			m_Render->Begin(cam);

			shade.Use();

			shade.SetUniformMat4("model", glm::mat4(1.f));

			cube.Draw();

			m_SceneManager->Render(&shade);

			m_Render->End();
		}
	}

	OpenGLRender* Engine::GetWindow() const
	{
		return m_Render.get();
	}
}
