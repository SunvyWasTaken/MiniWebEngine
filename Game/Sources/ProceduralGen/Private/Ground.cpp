// Sunset inc.

#include "Ground.h"

#include "Components/PhysicComponent.h"
#include "Components/RenderComponent.h"
#include "Components/TransformComponent.h"
#include "Drawable.h"
#include "Material.h"
#include "Meshes/Mesh.h"
#include "PlaneGen.h"
#include "ShaderLoader.h"
#include "Textures/TextureManager.h"

#include "Meshes/MeshLoader.h"


namespace Sunset
{
	void Ground::Init()
	{
		AddComponent<Sunset::TransformComponent>();
		auto* transComp = GetComponent<Sunset::TransformComponent>();
		transComp->SetPosition({ 0, 0, 0});

		Sunset::StaticVertices dt;
		Sunset::PlaneGen::Gen(dt, 10.f, 10.f, 100.f, 100.f);
		//Sunset::AlgoProcedural::PerlinNoise(dt, 0.5f, 2.f);
		//Sunset::AlgoProcedural::Erosion(dt, 100.f, 100.f);
		//Sunset::PlaneGen::ProcessNormal(dt);

		std::shared_ptr<Sunset::Shader> shader = Sunset::ShaderLoader::Load("Base", "Ressources/Shaders/vShader.glsl", "Ressources/Shaders/fShader.glsl");
		Sunset::AnyTexture groundTexture = Sunset::TextureLoader::Load("Ressources/Gravel.jpg");
		Sunset::AnyTexture grassTexture = Sunset::TextureLoader::Load("Ressources/Grass.jpg");
		Sunset::AnyTexture snowTexture = Sunset::TextureLoader::Load("Ressources/Snow.jpg");

		std::shared_ptr<Sunset::Material> mat = std::make_shared<Sunset::Material>(shader, groundTexture);
		mat->AddTexture(grassTexture);
		mat->AddTexture(snowTexture);

		//std::shared_ptr<Sunset::StaticMesh> vd = std::make_shared<Sunset::StaticMesh>(dt);
		//std::shared_ptr<Sunset::Drawable> drawableGround = std::make_shared<Sunset::Drawable>(vd, mat);
		//AddComponent<Sunset::RenderComponent>(drawableGround);

		AddComponent<Sunset::PhysicComponent>(Sunset::PhyscShape::Plane{ transComp->GetPosition() });
	}

	void Pig::Init()
	{
		AddComponent<TransformComponent>(glm::vec3{0, 1, 0});
		auto* transComp = GetComponent<TransformComponent>();

		float scale = 1.f;
		SkeletalVertices vo;

		transComp->SetSize({scale, scale, scale});

		std::shared_ptr<Shader> shader = ShaderLoader::Load("Pig", "Ressources/Shaders/vShaderCharacter.glsl", "Ressources/Shaders/fShaderCharacter.glsl");
		Sunset::AnyTexture pigTexture = TextureLoader::Load("Ressources/pig/T_Pig_Base_Color.png");

		std::shared_ptr<Material> mat = std::make_shared<Material>(shader, pigTexture);
		//std::shared_ptr<SkeletalMesh> m = std::make_shared<SkeletalMesh>(vo);
		//std::shared_ptr<Drawable> drawablePig = std::make_shared<Drawable>(m, mat);
		//AddComponent<RenderComponent>(drawablePig);
	}
}
