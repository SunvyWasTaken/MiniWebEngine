#include "Object.h"
#include "RenderObject.h"

#include <iostream>

namespace Sunset
{
	constexpr double G = -9.81;

	TransformComponent::TransformComponent(const glm::vec2& loc)
		: location(loc)
		, size(glm::vec3{0.1, 0.1, 1.0})
		, bStatic(false)
	{
	}

	PhysicComponent::PhysicComponent()
		: velocity(glm::vec2{0.0, 0.0})
		, speed(1.f)
	{
	}

	void PhysicComponent::ApplyPhysic(const double deltatime, TransformComponent& transform)
	{
		constexpr glm::vec2 gravity{ 0.0, G };

		velocity.x += gravity.x * deltatime;
		velocity.y += gravity.y * deltatime;

		transform.location.x += velocity.x * deltatime;
		transform.location.y += velocity.y * deltatime;
	}

	bool CollisionTest::Intersect(TransformComponent& A, TransformComponent& B)
	{
		glm::vec2 aMin = A.location - A.size * 0.5f;
		glm::vec2 aMax = A.location + A.size * 0.5f;

		glm::vec2 bMin = B.location - B.size * 0.5f;
		glm::vec2 bMax = B.location + B.size * 0.5f;

		bool overlapX = aMin.x <= bMax.x && aMax.x >= bMin.x;
		bool overlapY = aMin.y <= bMax.y && aMax.y >= bMin.y;

		return overlapX && overlapY;
	}

	void CollisionTest::ResolveCollision(TransformComponent& A, TransformComponent& B, float deltatime)
	{
		glm::vec2 delta = B.location - A.location;
		glm::vec2 overlap = (A.size + B.size) * 0.5f - glm::abs(delta);

		if (overlap.x < overlap.y)
		{
			float direction = delta.x < 0 ? -1.0f : 1.0f;
			if (!A.bStatic)
				A.location.x -= direction * overlap.x * 0.5f;
			if (!B.bStatic)
				B.location.x += direction * overlap.x * 0.5f;
		}
		else
		{
			float direction = delta.y < 0 ? -1.0f : 1.0f;
			if (!A.bStatic)
				A.location.y -= direction * overlap.y * 0.5f;
			if (!B.bStatic)
				B.location.y += direction * overlap.y * 0.5f;
		}
	}

}
