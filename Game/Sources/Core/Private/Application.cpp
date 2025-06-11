// Sunset inc.

#include "AlgoProcedural.h"
#include "Application.h"
#include "Camera.h"
#include "Components/RenderComponent.h"
#include "Components/TransformComponent.h"
#include "Entity.h"
#include "Inputs.h"
#include "OpenGLWindow.h"
#include "PlaneGen.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Shaders.h"
#include "VertexObject.h"

#include "Drawable.h"

struct Menu : public Sunset::Scene
{
	Menu()
	{ }

	~Menu()
	{ }

	virtual void Begin() override;

	virtual void Update(const float deltatime) override;
};

Sunset::Engine* Sunset::Engine::m_Engine = nullptr;

namespace
{
	bool bIsAppOpen = false;

	Sunset::Camera cam;

	std::unique_ptr<Sunset::OpenGLRender> m_Render = nullptr;

	std::unique_ptr<Sunset::SceneManager<Menu>> m_SceneManager = nullptr;
}

void Menu::Begin()
{
	Sunset::Scene::Begin();

	Sunset::Entity Ground = Sunset::Engine::GetWorld()->CreateEntity();
	Ground.AddComponent<Sunset::TransformComponent>();
	Sunset::Object dt;
	Sunset::PlaneGen::Gen(dt, 10.f, 10.f, 100.f, 100.f);
	Sunset::AlgoProcedural::PerlinNoise(dt, 0.5f, 5.f);
	Sunset::AlgoProcedural::Erosion(dt, 100.f, 100.f);
	//Sunset::PlaneGen::ApplyWaveToTerrain(dt);
	Sunset::PlaneGen::ProcessNormal(dt);
	std::shared_ptr<Sunset::VertexObject> vd = std::make_shared<Sunset::VertexObject>(dt);
	Ground.AddComponent<Sunset::RenderComponent>(vd);
}

void Menu::Update(const float deltatime)
{
	Scene::Update(deltatime);

	if (Sunset::Inputs::IsKey(68))
	{
		glm::vec3 dir = cam.GetCameraRightVector();
		cam.AddPosition(dir * deltatime);
	}
	if (Sunset::Inputs::IsKey(65))
	{
		glm::vec3 dir = cam.GetCameraRightVector();
		cam.AddPosition(-dir * deltatime);
	}
	if (Sunset::Inputs::IsKey(83))
	{
		glm::vec3 dir = cam.GetCameraForwardVector();
		cam.AddPosition(-dir * deltatime);
	}
	if (Sunset::Inputs::IsKey(87))
	{
		glm::vec3 dir = cam.GetCameraForwardVector();
		cam.AddPosition(dir * deltatime);
	}
	if (Sunset::Inputs::IsKey(69))
	{
		glm::vec3 dir = cam.GetCameraUpVector();
		cam.AddPosition(dir * deltatime);
	}
	if (Sunset::Inputs::IsKey(81))
	{
		glm::vec3 dir = cam.GetCameraUpVector();
		cam.AddPosition(-dir * deltatime);
	}

	glm::vec3 rot = cam.GetRotation();
	glm::vec2 mosM = Sunset::Inputs::MouseMovement() * 0.1f;
	rot.x += mosM.x;
	rot.y -= mosM.y;
	rot.y = glm::clamp(rot.y, -89.0f, 89.0f);
	cam.SetRotation({ rot.x, rot.y, 0.f });
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

		Shader shade{"../../Game/Sources/Shaders/vShader.glsl", "../../Game/Sources/Shaders/fShader.glsl"};

		while (bIsAppOpen)
		{
			std::chrono::steady_clock::time_point _now = std::chrono::steady_clock::now();
			std::chrono::duration<float> delta = _now - _past;
			_past = _now;
			float Deltatime = delta.count();

			if (Inputs::IsKey(256))
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
