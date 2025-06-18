// Sunset inc.

#include "Skybox.h"

#include "Components/TransformComponent.h"
#include "Components/RenderComponent.h"

#include "Mesh.h"
#include "PlaneGen.h"
#include "ShaderLoader.h"
#include "Material.h"
#include "TextureManager.h"
#include "Drawable.h"

namespace
{
	Sunset::VertexObject data = {
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

namespace Sunset
{
	void SkyBox::Init()
	{
		AddComponent<Sunset::TransformComponent>();
		Sunset::TransformComponent* transComp = GetComponent<Sunset::TransformComponent>();
		transComp->SetSize({100, 100, 100});

		std::shared_ptr<Sunset::Shader> SkyBoxshader = Sunset::ShaderLoader::Load("Skybox", "Ressources/Shaders/vShaderSkyBox.glsl", "Ressources/Shaders/fShaderSkyBox.glsl");
		Sunset::AnyTexture CubeTexture = Sunset::TextureLoader::Load("skybox", TextureList);
		std::shared_ptr<Sunset::Material> matSkyBox = std::make_shared<Sunset::Material>(SkyBoxshader, CubeTexture);

		std::shared_ptr<Sunset::Mesh> SkyDataBox = std::make_shared<Sunset::Mesh>(data);
		std::shared_ptr<Sunset::Drawable> drawableSky = std::make_shared<Sunset::Drawable>(SkyDataBox, matSkyBox);
		AddComponent<Sunset::RenderComponent>(drawableSky);
	}
}
