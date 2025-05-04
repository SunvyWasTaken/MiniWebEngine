#pragma once

#include "Object.h"
#include "ShapeDetection.h"

namespace Sunset
{
	class TransformComponent;
}

class Ennemy : public Sunset::Entity
{
public:

	BODY(Ennemy)

	virtual void Begin() override;

	virtual void Update(double deltatime) override;

	void AddPosition(const glm::vec2& pos);

	glm::vec2 GetLocation() const;

private:

	Sunset::TransformComponent* transComp = nullptr;
};

class Bee : public Sunset::Entity
{
public:

	BODY(Bee)

	virtual void Begin() override;

	virtual void Update(double deltatime);

	void Attack(const std::vector<Ennemy>& targets);

	void AddPosition(const glm::vec2& pos);

	glm::vec2 GetLocation() const;

private:

	Ennemy* NearstEnnemy();

private:
	friend class GameWorld;
	Sunset::TransformComponent* transComp = nullptr;
	float BeeSpeed = 4.f;
	ShapeModel::Type type;
	bool IsInCooldown = false;
	bool isAttacking = false;
	glm::vec2 targetLocation;
	glm::vec2 initPosition;
};