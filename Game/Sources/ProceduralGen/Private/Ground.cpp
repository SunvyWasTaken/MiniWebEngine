// Sunset inc.

#include "Ground.h"

#include "Components/TransformComponent.h"
#include "Components/RenderComponent.h"
#include "Components/PhysicComponent.h"

#include "VertexObject.h"
#include "PlaneGen.h"
#include "ShaderLoader.h"
#include "Material.h"
#include "TextureManager.h"
#include "Drawable.h"


namespace Sunset
{
	void Ground::Init()
	{
		AddComponent<Sunset::TransformComponent>();
		auto* transComp = GetComponent<Sunset::TransformComponent>();
		transComp->SetPosition({ 0, 0, 0});

		Sunset::Object dt;
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

		std::shared_ptr<Sunset::VertexObject> vd = std::make_shared<Sunset::VertexObject>(dt);
		std::shared_ptr<Sunset::Drawable> drawableGround = std::make_shared<Sunset::Drawable>(vd, mat);
		AddComponent<Sunset::RenderComponent>(drawableGround);

		AddComponent<Sunset::PhysicComponent>(Sunset::PhyscShape::Plane{ transComp->GetPosition() });
	}
}
