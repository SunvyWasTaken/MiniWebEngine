// Sunset inc.

#include "Components/PhysicComponent.h"

#include "physx/PxPhysicsAPI.h"

namespace Sunset
{
	PhysicComponent::PhysicComponent(const PhyscShape::Type& shape)
		: m_Shape(shape)
		, actor(nullptr)
	{
		actor = Sunset::PhysicSystem::CreateStaticShape(shape);
	}

	PhysicComponent::~PhysicComponent()
	{
	}

	glm::vec3 PhysicComponent::GetPosition() const {
		physx::PxTransform transform = actor->getGlobalPose();
		return glm::vec3(transform.p.x, transform.p.y, transform.p.z);
	}

	glm::quat PhysicComponent::GetRotation() const {
		physx::PxTransform transform = actor->getGlobalPose();
		return glm::quat(transform.q.w, transform.q.x, transform.q.y, transform.q.z);
	}
}
