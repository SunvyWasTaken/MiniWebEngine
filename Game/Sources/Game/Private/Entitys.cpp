#include "Entitys.h"
#include "Components.h"
#include "RenderObject.h"
#include "World.h"

namespace
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(-10, 10);
}

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
	if (isAttacking && !isStun)
	{
		AddPosition({ 0, -1 * deltatime });
		glm::vec2 targetLoc{GetLocation().x, -1};
		float dist = glm::length(targetLoc - GetLocation());
		if (dist <= 0.05f)
		{
			isAttacking = false;
			if (GameWorld* w = static_cast<GameWorld*>(GetWorld()))
			{
				w->LostLife();
			}
		}
	}
}

void Ennemy::AddPosition(const glm::vec2& pos)
{
	transComp->location += pos;
}

void Ennemy::SetLocation(const glm::vec2& pos)
{
	transComp->location = pos;
}

glm::vec2 Ennemy::GetLocation() const
{
	return transComp->location;
}

void Ennemy::Attack()
{
	isAttacking = true;
	int loc = dis(gen);
	float targetLoc = 1.f/loc;

	SetLocation({targetLoc, 1});
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

	float dt = deltatime;

	if (isAttacking)
	{
		BeeSpeed = 4.f;

		glm::vec2 dir = targetLocation - GetLocation();
		float dist = glm::length(dir);
		glm::vec2 normDir = glm::normalize(dir);
		if (dist >= 0.05f)
			AddPosition(normDir * BeeSpeed * dt);
		else
			isAttacking = false;
	}
	else if (IsInCooldown)
	{
		BeeSpeed = 1.f;

		glm::vec2 dir = initPosition - GetLocation();
		float dist = glm::length(dir);
		glm::vec2 normDir = glm::normalize(dir);
		if (dist >= 0.01f)
			AddPosition(normDir * BeeSpeed * dt);
		else
			IsInCooldown = false;
	}

}

void Bee::Attack(const std::vector<Ennemy>& targets)
{
	if (IsInCooldown)
		return;

	IsInCooldown = true;

	if (targets.empty())
	{
		initPosition = GetLocation();
		isAttacking = true;
		targetLocation = glm::vec2{0, 1};
		return;
	}
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
