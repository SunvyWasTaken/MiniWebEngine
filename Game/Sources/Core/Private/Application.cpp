// Sunset inc.

#include "Application.h"
#include "Camera.h"
#include "Components/CollisionComponent.h"
#include "Components/RenderComponent.h"
#include "Components/ScriptComponent.h"
#include "Components/TransformComponent.h"
#include "Entity.h"
#include "Inputs.h"
#include "Log.h"
#include "OpenGLWindow.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Shaders.h"
#include "VertexObject.h"

struct Menu : public Sunset::Scene
{
	Menu() = default;

	~Menu()
	{
	}

	virtual void Update(const float deltatime) override
	{
		Scene::Update(deltatime);
	}

	Sunset::VertexObject cube;
};

Sunset::Engine* Sunset::Engine::m_Engine = nullptr;

namespace
{

	bool bIsAppOpen = false;

	Sunset::Camera cam;

	std::unique_ptr<Sunset::OpenGLRender> m_Render = nullptr;

	std::unique_ptr<Sunset::SceneManager<Menu>> m_SceneManager = nullptr;

	Sunset::Object data = { {Sunset::Vertex	{{ 0.0,  0.5, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.0} },
											{{-0.5, -0.5, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0} },
											{{ 0.5, -0.5, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0} }}
											, {0, 1, 2}};
}

namespace Sunset
{
	Engine::Engine()
	{
		bIsAppOpen = true;
		Log::Init();
		ENGINE_LOG_TRACE("Welcome to the engine create by Neo")

		m_Render = std::make_unique<OpenGLRender>();
		m_SceneManager = std::make_unique<Sunset::SceneManager<Menu>>();
		m_SceneManager->GetScene()->Begin();
	}

	Engine::~Engine()
	{
		m_SceneManager.reset();
		m_Render.reset();
		ENGINE_LOG_TRACE("Engine Destroyed")
	}

	void Engine::Destroy()
	{
		delete m_Engine;
	}

	void Engine::Run()
	{
		ENGINE_LOG_TRACE("Start Run")
		std::chrono::steady_clock::time_point _past = std::chrono::steady_clock::now();

		Sunset::VertexObject cube{ data };

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

			m_SceneManager->Update(Deltatime);

			m_Render->Begin(cam);

			shade.Use();

			m_SceneManager->Render(&shade);

			m_Render->End();
		}
	}

	OpenGLRender* Engine::GetWindow() const
	{
		return m_Render.get();
	}

	Scene* Engine::GetWorld()
	{
		return m_SceneManager->GetScene();
	}

	void Engine::LogicLoop(const float deltatime)
	{
		
	}

	Camera& Engine::GetCam() const
	{
		return cam;
	}
}
