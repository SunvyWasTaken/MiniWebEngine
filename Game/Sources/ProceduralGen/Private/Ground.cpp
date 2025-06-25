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

#include "Meshes/Vertex.h"

#include "Meshes/SkeletalMesh.h"

#include "Meshes/MeshLoader.h"
#include "Animation/AnimationLoader.h"

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

		Sunset::StaticMeshData dt;
		Sunset::PlaneGen::Gen(dt, 100.f, 100.f, 100.f, 100.f);
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

		AddComponent<Sunset::PhysicComponent>(Sunset::PhyscShape::Cube{ transComp->GetPosition(), planeRotation, {50.f, 0.1f, 50.f}});
	}

	void Pig::Init()
	{
		AddComponent<TransformComponent>(glm::vec3{ 0, 200, 0 });
		auto* transComp = GetComponent<TransformComponent>();

		float scale = 1.f;

		transComp->SetSize({ scale, scale, scale });

		std::shared_ptr<Shader> shader = ShaderLoader::Load("Pig", "Ressources/Shaders/vShaderCharacter.glsl", "Ressources/Shaders/fShaderCharacter.glsl");
		Sunset::AnyTexture pigTexture = TextureLoader::Load("Ressources/Cisailleur/T_Cisailleur_D.png");

		std::shared_ptr<Material> mat = std::make_shared<Material>(shader, pigTexture);
		std::shared_ptr<Meshes> mesh = MeshLoader::LoadSkeletalMesh("Ressources/Hiro/A_GardeIdle.fbx");
		Skeletal& skel = std::get_if<SkeletalMesh>(mesh.get())->GetSkel();
		std::shared_ptr<AnimationClip> anim = AnimLoader::LoadAnimation("Ressources/Hiro/A_GardeIdle.fbx", skel.bones);
		std::get_if<SkeletalMesh>(mesh.get())->AddAnimation(anim);
		std::shared_ptr<Drawable> drawablePig = std::make_shared<Drawable>(mesh, mat);
		AddComponent<RenderComponent>(drawablePig);

		AddComponent<PhysicComponent>(PhyscShape::Capsule{transComp->GetPosition(), transComp->GetRotation(), 32.f, 40.f}, false);
	}
}
