#pragma once

#include "Object.h"

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

	void AddPosition(const glm::vec2& pos);

	glm::vec2 GetLocation() const;

private:

	Ennemy* NearstEnnemy();

private:

	Sunset::TransformComponent* transComp = nullptr;
	float BeeSpeed = 4.f;
	// Geometry
};