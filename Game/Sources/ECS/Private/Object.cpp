#include "Object.h"
#include "RenderObject.h"

namespace Sunset
{
	constexpr double G = -9.81;

	TransformComponent::TransformComponent(const glm::vec2& loc)
		: location(loc)
		, size(glm::vec3{0.1, 0.1, 1.0})
		, rot(0.f)
		, bStatic(false)
	{
	}

	PhysicComponent::PhysicComponent()
		: velocity(glm::vec2{0.0, 0.0})
		, maxSpeed(1.5f)
		, speed(1.f)
	{
	}

	PhysicComponent::~PhysicComponent()
	{
	}

	void PhysicComponent::ApplyPhysic(const double deltatime, TransformComponent& transform)
	{
		constexpr glm::vec2 gravity{ 0.0, G };

		velocity.x += gravity.x * deltatime;
		velocity.y += gravity.y * deltatime;

		if (velocity.x >= maxSpeed)
			velocity.x = maxSpeed;
		if (velocity.y >= maxSpeed)
			velocity.y = maxSpeed;

		transform.location.x += velocity.x * deltatime;
		transform.location.y += velocity.y * deltatime;
	}

	void PhysicComponent::AddImpulse(const glm::vec2& dir, float force)
	{
		velocity = dir * force;
	}

	bool CollisionTest::Intersect(const Entity& A, const Entity& B, World& world)
	{
		auto [transCompA, colliCompA] = world.try_get<TransformComponent, CollisionComponent>(A);
		auto [transCompB, colliCompB] = world.try_get<TransformComponent, CollisionComponent>(B);
		if (!transCompA || !colliCompA || !transCompB || !colliCompB)
			return false;

		return std::visit(overload{
			[&](const CollisionShape::Square& a, const CollisionShape::Square& b)
			{
				glm::vec2 aMin = transCompA->location - transCompA->size * 0.5f;
				glm::vec2 aMax = transCompA->location + transCompA->size * 0.5f;

				glm::vec2 bMin = transCompB->location - transCompB->size * 0.5f;
				glm::vec2 bMax = transCompB->location + transCompB->size * 0.5f;

				bool overlapX = aMin.x <= bMax.x && aMax.x >= bMin.x;
				bool overlapY = aMin.y <= bMax.y && aMax.y >= bMin.y;

				return overlapX && overlapY;
			},
			[&](const CollisionShape::Square& a, const CollisionShape::Sphere& b)
			{
				return false;
			},
			[&](const CollisionShape::Sphere& a, const CollisionShape::Square& b)
			{
				return false;
			},
			[&](const CollisionShape::Sphere& a, const CollisionShape::Sphere& b)
			{
				return false;
			}
		}, colliCompA->type, colliCompB->type);
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

	void CollisionComponent::Collision()
	{
		if (OnCollision)
		{
			OnCollision();
		}
	}

}
