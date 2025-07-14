// Sunset inc.

#include "Prefab/Pawn.h"

#include "Animation/AnimationLoader.h"
#include "CameraManager.h"
#include "Components/CameraComponent.h"
#include "Components/PhysicComponent.h"
#include "Components/RenderComponent.h"
#include "Components/TransformComponent.h"
#include "Material.h"
#include "Meshes/Drawable.h"
#include "Meshes/MeshLoader.h"
#include "Meshes/SkeletalMesh.h"
#include "Meshes/StaticMesh.h"
#include "ShaderLoader.h"
#include "Textures/TextureManager.h"

namespace Sunset
{
	void Pawn::Init()
	{
		m_TransComp = AddComponent<TransformComponent>();
		m_PhysComp = AddComponent<PhysicComponent>(PhyscShape::Capsule{ m_TransComp->GetPosition(), m_TransComp->GetRotation(), 0.32f, 1.8f }, false);
		m_CameraComp = AddComponent<CameraComponent>();
		m_CameraComp->SetCamera(CameraManager::GetActiveCamera());

		m_TransComp->SetSize({0.01f, 0.01f, 0.01f});

		std::shared_ptr<Shader> shader = ShaderLoader::Load("Pig", "Ressources/Shaders/vShaderCharacter.glsl", "Ressources/Shaders/fShaderCharacter.glsl");
		Sunset::AnyTexture pigTexture = TextureLoader::Load("Ressources/Cisailleur/T_Cisailleur_D.png");

		std::shared_ptr<Material> mat = std::make_shared<Material>(shader, pigTexture);
		std::shared_ptr<Meshes> mesh = MeshLoader::LoadSkeletalMesh("Ressources/Hiro/A_GardeIdle.fbx");
		Skeletal& skel = std::get_if<SkeletalMesh>(mesh.get())->GetSkel();
		std::shared_ptr<AnimationClip> anim = AnimLoader::LoadAnimation("Ressources/Hiro/A_GardeIdle.fbx", skel.bones);
		std::get_if<SkeletalMesh>(mesh.get())->AddAnimation(anim);
		std::shared_ptr<Drawable> drawablePig = std::make_shared<Drawable>(mesh, mat);
		AddComponent<RenderComponent>(drawablePig);
	}

	void Pawn::AddPosition(const glm::vec3& dir)
	{
		m_PhysComp->AddForce(dir);
	}

	glm::vec3 Pawn::GetPosition() const
	{
		return m_TransComp->GetPosition();
	}

}
