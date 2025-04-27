#include "Components.h"
#include "RenderObject.h"

namespace Sunset
{
	constexpr double G = -9.81;

	TransformComponent::TransformComponent(const glm::vec2& loc)
		: location(loc)
		, size(glm::vec3{ 0.1, 0.1, 1.0 })
		, rot(0.f)
		, bStatic(false)
	{
	}

	RenderObjectComponent::RenderObjectComponent(RenderObject* obj)
		: data(std::unique_ptr<RenderObject>(obj))
	{
	}

	RenderObjectComponent::~RenderObjectComponent()
	{
	}

	PhysicComponent::PhysicComponent()
		: velocity(glm::vec2{ 0.0, 0.0 })
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
}
