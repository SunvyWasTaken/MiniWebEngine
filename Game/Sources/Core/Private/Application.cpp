// Sunset inc.

#include "Application.h"
#include "Camera.h"
#include "Components/RenderComponent.h"
#include "Components/TransformComponent.h"
#include "Entity.h"
#include "Inputs.h"
#include "Log.h"
#include "OpenGLWindow.h"
#include "PlaneGen.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Shaders.h"
#include "VertexObject.h"

struct Menu : public Sunset::Scene
{
	Menu()
	{ }

	~Menu()
	{
	}

	virtual void Update(const float deltatime) override
	{
		Scene::Update(deltatime);
	}
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

		Entity Ground = GetWorld()->CreateEntity();
		Ground.AddComponent<TransformComponent>();
		Object dt;
		PlaneGen::Gen(dt, 10.f, 10.f, 100.f, 100.f);
		VertexObject vd{dt};
		Ground.AddComponent<RenderComponent>(&vd);

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
				glm::vec3 dir = cam.GetCameraRightVector();
				cam.AddPosition(-dir * Deltatime);
			}
			if (Inputs::IsKey(65))
			{
				glm::vec3 dir = cam.GetCameraRightVector();
				cam.AddPosition(dir * Deltatime);
			}
			if (Inputs::IsKey(83))
			{
				glm::vec3 dir = cam.GetCameraForwardVector();
				cam.AddPosition(dir * Deltatime);
			}
			if (Inputs::IsKey(87))
			{
				glm::vec3 dir = cam.GetCameraForwardVector();
				cam.AddPosition(-dir * Deltatime);
			}
			if (Inputs::IsKey(69))
			{
				glm::vec3 dir = cam.GetCameraUpVector();
				cam.AddPosition(dir * Deltatime);
			}
			if (Inputs::IsKey(81))
			{
				glm::vec3 dir = cam.GetCameraUpVector();
				cam.AddPosition(-dir * Deltatime);
			}

			glm::vec3 rot = cam.GetRotation();
			glm::vec2 mosM = Inputs::MouseMovement() * 0.1f;
			cam.SetRotation({rot.x - mosM.x, rot.y + mosM.y, 0.f});

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
