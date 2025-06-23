// Sunset inc.

#include "Application.h"
#include "Camera.h"
#include "Inputs.h"
#include "OpenGLWindow.h"
#include "Scene.h"
#include "SceneManager.h"
#include "ShaderLoader.h"
#include "Shaders.h"
#include "Texts/FontLoader.h"

#include "Ground.h"
#include "Skybox.h"

struct Menu : public Sunset::Scene
{
	Menu()
		: Scene()
	{ }

	virtual ~Menu()
	{ }

	void Begin();

	void Render();

	void Update(const float deltatime);

	std::shared_ptr<Sunset::Shader> shaderText = nullptr;
};

struct Terrain : public Sunset::Scene
{
	Terrain()
		: Scene()
	{}

	virtual ~Terrain()
	{}

	void Begin();

	void Update(const float deltatime);
};

Sunset::Engine* Sunset::Engine::m_Engine = nullptr;

namespace
{
	bool bIsAppOpen = false;

	Sunset::Camera cam;

	std::unique_ptr<Sunset::OpenGLRender> m_Render = nullptr;

	std::unique_ptr<Sunset::SceneManager<Menu, Terrain>> m_SceneManager = nullptr;
}

void Menu::Begin()
{
	Scene::Begin();

	Sunset::FontLoader::LoadFont("Ressources/Delius-Regular.ttf");

	shaderText = Sunset::ShaderLoader::Load("textShader", "Ressources/Shaders/vShaderText.glsl", "Ressources/Shaders/fShaderText.glsl");
}

void Menu::Render()
{
	Scene::Render();

	Sunset::FontLoader::RenderText(shaderText, "Press Space", 1280.f/2.f - 180.f, 720.f/2.f, 1.f, {1.f, 1.f, 1.f});
}

void Menu::Update(const float deltatime)
{
	Scene::Update(deltatime);
	if (Sunset::Inputs::IsKey(32))
	{
		m_SceneManager->LoadScene<Terrain>();
	}
}

void Terrain::Begin()
{
	Sunset::Scene::Begin();

	/// Ground
	//CreateEntity<Sunset::Ground>();
	CreateEntity<Sunset::SkyBox>();
	CreateEntity<Sunset::Pig>();

}

void Terrain::Update(const float deltatime)
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
		m_SceneManager = std::make_unique<Sunset::SceneManager<Menu, Terrain>>();
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

		while (bIsAppOpen && m_Render->IsOpen())
		{
			std::chrono::steady_clock::time_point _now = std::chrono::steady_clock::now();
			std::chrono::duration<float> delta = _now - _past;
			_past = _now;
			float Deltatime = delta.count();

			if (Inputs::IsKey(256))
			{
				bIsAppOpen = false;
			}

			if (Inputs::IsKey(90))
			{
				OpenGLRender::WireframeMode(true);
			}
			else
			{
				OpenGLRender::WireframeMode(false);
			}

			m_SceneManager->Update(Deltatime);

			m_Render->Begin(cam);

			m_SceneManager->Render();

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

	Camera& Engine::GetCam() const
	{
		return cam;
	}
}
