#include "Entitys.h"
#include "Components.h"
#include "RenderObject.h"

void Ennemy::Begin()
{
	Super::Begin();
	transComp = &AddComponent<Sunset::TransformComponent>(glm::vec2{ 0, 1 });
	AddComponent<Sunset::RenderObjectComponent>(new Sunset::Square({ 1, 3 }));
	lifetime = 1.f;
}

void Ennemy::Update(double deltatime)
{
	Super::Update(deltatime);
	AddPosition({ 0, -1 * deltatime });
}

void Ennemy::AddPosition(const glm::vec2& pos)
{
	transComp->location += pos;
}

glm::vec2 Ennemy::GetLocation() const
{
	return transComp->location;
}

void Bee::Begin()
{
	Super::Begin();
	transComp = &AddComponent<Sunset::TransformComponent>(glm::vec2{ 0, 0 });
	AddComponent<Sunset::RenderObjectComponent>(new Sunset::Square({ 0, 3 }));
}

void Bee::Update(double deltatime)
{
	Super::Update(deltatime);

}

void Bee::AddPosition(const glm::vec2& pos)
{
	transComp->location += pos;
}

glm::vec2 Bee::GetLocation() const
{
	return transComp->location;
}

Ennemy* Bee::NearstEnnemy()
{
	//if (ennemys.empty())
		return nullptr;

	//Ennemy* nearest = ennemys[0];
	//float minDist = glm::length(nearest->GetLocation() - GetLocation());

	//for (Ennemy* ennemy : ennemys)
	//{
	//	float currDist = glm::length(ennemy->GetLocation() - GetLocation());
	//	if (minDist <= currDist)
	//	{
	//		minDist = currDist;
	//		nearest = ennemy;
	//	}
	//}

	//return nearest;
}
