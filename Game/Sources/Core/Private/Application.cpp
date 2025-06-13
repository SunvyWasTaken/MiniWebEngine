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

#include "Material.h"
#include "Drawable.h"
#include "ShaderLoader.h"

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

	Sunset::Object data = {
	{
		{{-0.5f, -0.5f,  0.5f}, { 0,  0,  1}, {0.0f, 0.0f}}, // 0
		{{ 0.5f, -0.5f,  0.5f}, { 0,  0,  1}, {1.0f, 0.0f}}, // 1
		{{ 0.5f,  0.5f,  0.5f}, { 0,  0,  1}, {1.0f, 1.0f}}, // 2
		{{-0.5f,  0.5f,  0.5f}, { 0,  0,  1}, {0.0f, 1.0f}}, // 3

		{{ 0.5f, -0.5f,  0.5f}, { 1,  0,  0}, {0.0f, 0.0f}}, // 4
		{{ 0.5f, -0.5f, -0.5f}, { 1,  0,  0}, {1.0f, 0.0f}}, // 5
		{{ 0.5f,  0.5f, -0.5f}, { 1,  0,  0}, {1.0f, 1.0f}}, // 6
		{{ 0.5f,  0.5f,  0.5f}, { 1,  0,  0}, {0.0f, 1.0f}}, // 7

		{{ 0.5f, -0.5f, -0.5f}, { 0,  0, -1}, {0.0f, 0.0f}}, // 8
		{{-0.5f, -0.5f, -0.5f}, { 0,  0, -1}, {1.0f, 0.0f}}, // 9
		{{-0.5f,  0.5f, -0.5f}, { 0,  0, -1}, {1.0f, 1.0f}}, //10
		{{ 0.5f,  0.5f, -0.5f}, { 0,  0, -1}, {0.0f, 1.0f}}, //11

		{{-0.5f, -0.5f, -0.5f}, {-1,  0,  0}, {0.0f, 0.0f}}, //12
		{{-0.5f, -0.5f,  0.5f}, {-1,  0,  0}, {1.0f, 0.0f}}, //13
		{{-0.5f,  0.5f,  0.5f}, {-1,  0,  0}, {1.0f, 1.0f}}, //14
		{{-0.5f,  0.5f, -0.5f}, {-1,  0,  0}, {0.0f, 1.0f}}, //15

		{{-0.5f,  0.5f,  0.5f}, { 0,  1,  0}, {0.0f, 0.0f}}, //16
		{{ 0.5f,  0.5f,  0.5f}, { 0,  1,  0}, {1.0f, 0.0f}}, //17
		{{ 0.5f,  0.5f, -0.5f}, { 0,  1,  0}, {1.0f, 1.0f}}, //18
		{{-0.5f,  0.5f, -0.5f}, { 0,  1,  0}, {0.0f, 1.0f}}, //19

		{{-0.5f, -0.5f, -0.5f}, { 0, -1,  0}, {0.0f, 0.0f}}, //20
		{{ 0.5f, -0.5f, -0.5f}, { 0, -1,  0}, {1.0f, 0.0f}}, //21
		{{ 0.5f, -0.5f,  0.5f}, { 0, -1,  0}, {1.0f, 1.0f}}, //22
		{{-0.5f, -0.5f,  0.5f}, { 0, -1,  0}, {0.0f, 1.0f}}, //23
	},
	{
		0, 1, 2,  2, 3, 0,
		4, 5, 6,  6, 7, 4,
		8, 9,10, 10,11, 8,
	   12,13,14, 14,15,12,
	   16,17,18, 18,19,16,
	   20,21,22, 22,23,20
	}
	};

	std::array<std::string_view, 6> TextureList{
												  "Ressources/skybox/right.jpg"
												, "Ressources/skybox/left.jpg"
												, "Ressources/skybox/top.jpg"
												, "Ressources/skybox/bottom.jpg"
												, "Ressources/skybox/front.jpg"
												, "Ressources/skybox/back.jpg"
	};
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

	std::shared_ptr<Sunset::Shader> shader = Sunset::ShaderLoader::Load("Base", "Ressources/Shaders/vShader.glsl", "Ressources/Shaders/fShader.glsl");

	Sunset::AnyTexture groundTexture = Sunset::TextureLoader::Load("Ressources/Gravel.jpg");
	Sunset::AnyTexture grassTexture = Sunset::TextureLoader::Load("Ressources/Grass.jpg");
	Sunset::AnyTexture snowTexture = Sunset::TextureLoader::Load("Ressources/Snow.jpg");
	std::shared_ptr<Sunset::Material> mat = std::make_shared<Sunset::Material>(shader, groundTexture);
	mat->AddTexture(grassTexture);
	mat->AddTexture(snowTexture);
	std::shared_ptr<Sunset::Drawable> drawableGround = std::make_shared<Sunset::Drawable>(vd, mat);
	Ground.AddComponent<Sunset::RenderComponent>(drawableGround);


	std::shared_ptr<Sunset::Shader> SkyBoxshader = Sunset::ShaderLoader::Load("Skybox", "Ressources/Shaders/vShaderSkyBox.glsl", "Ressources/Shaders/fShaderSkyBox.glsl");
	Sunset::AnyTexture CubeTexture = Sunset::TextureLoader::Load("skybox", TextureList);
	std::shared_ptr<Sunset::Material> matSkyBox = std::make_shared<Sunset::Material>(SkyBoxshader, CubeTexture);
	Sunset::Entity SkyBox = Sunset::Engine::GetWorld()->CreateEntity();
	SkyBox.AddComponent<Sunset::TransformComponent>();
	Sunset::TransformComponent* transComp = SkyBox.GetComponent<Sunset::TransformComponent>();
	transComp->SetSize({100, 100, 100});
	std::shared_ptr<Sunset::VertexObject> SkyDataBox = std::make_shared<Sunset::VertexObject>(data);
	std::shared_ptr<Sunset::Drawable> drawableSky = std::make_shared<Sunset::Drawable>(SkyDataBox, matSkyBox);
	SkyBox.AddComponent<Sunset::RenderComponent>(drawableSky);
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

	void Engine::LogicLoop(const float deltatime)
	{
		
	}

	Camera& Engine::GetCam() const
	{
		return cam;
	}
}
