// Sunset inc.

#include "Ground.h"

#include "Components/PhysicComponent.h"
#include "Components/RenderComponent.h"
#include "Components/TransformComponent.h"
#include "Meshes/Drawable.h"
#include "Material.h"
#include "PlaneGen.h"
#include "ShaderLoader.h"
#include "Textures/TextureManager.h"

namespace
{
}

namespace Sunset
{
	void Ground::Init()
	{
		glm::quat planeRotation = glm::identity<glm::quat>();
		AddComponent<TransformComponent>(glm::vec3{0, 0, 0}, planeRotation);
		auto* transComp = GetComponent<Sunset::TransformComponent>();

		float PlaneSize = 2000.f;

		Sunset::StaticMeshData dt;
		Sunset::PlaneGen::Gen(dt, PlaneSize, PlaneSize, 100.f, 100.f);
		//Sunset::AlgoProcedural::PerlinNoise(dt, 0.5f, 2.f);
		//Sunset::AlgoProcedural::Erosion(dt, 100.f, 100.f);
		Sunset::PlaneGen::ProcessNormal(dt);

		std::shared_ptr<Sunset::Shader> shader = Sunset::ShaderLoader::Load("Base", "Ressources/Shaders/vShader.glsl", "Ressources/Shaders/fShader.glsl");
		Sunset::AnyTexture groundTexture = Sunset::TextureLoader::Load("Ressources/Gravel.jpg");
		Sunset::AnyTexture grassTexture = Sunset::TextureLoader::Load("Ressources/Grass.jpg");
		Sunset::AnyTexture snowTexture = Sunset::TextureLoader::Load("Ressources/Snow.jpg");

		std::shared_ptr<Sunset::Material> mat = std::make_shared<Sunset::Material>(shader, groundTexture);
		mat->AddTexture(grassTexture);
		mat->AddTexture(snowTexture);

		StaticMesh sm;
		sm.AddSubMesh(dt);
		std::shared_ptr<Sunset::Meshes> vd = std::make_shared<Sunset::Meshes>(std::move(sm));
		std::shared_ptr<Sunset::Drawable> drawableGround = std::make_shared<Sunset::Drawable>(vd, mat);
		AddComponent<Sunset::RenderComponent>(drawableGround);

		AddComponent<Sunset::PhysicComponent>(Sunset::PhyscShape::Cube{ transComp->GetPosition(), planeRotation, {PlaneSize / 2.f, 0.1f, PlaneSize/2.f}});
	}
}
