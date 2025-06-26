// Sunset inc.

#include "Components/PhysicComponent.h"

#include "physx/PxPhysicsAPI.h"

namespace Sunset
{
	PhysicComponent::PhysicComponent(const PhyscShape::Type& shape, bool IsStatic)
		: m_Shape(shape)
		, m_Actor((IsStatic ? Sunset::PhysicSystem::CreateStaticShape(shape) : Sunset::PhysicSystem::CreateDynamicShape(shape)))
	{
		ENGINE_LOG_TRACE("Physic Component create")
	}

	PhysicComponent::~PhysicComponent()
	{
		ENGINE_LOG_TRACE("Physic Component destroy")
	}

	glm::vec3 PhysicComponent::GetPosition() const
	{
		physx::PxTransform transform = m_Actor->getGlobalPose();
		return glm::vec3(transform.p.x, transform.p.y, transform.p.z);
	}

	glm::quat PhysicComponent::GetRotation() const
	{
		physx::PxTransform transform = m_Actor->getGlobalPose();
		return glm::quat(transform.q.w, transform.q.x, transform.q.y, transform.q.z);
	}

	void PhysicComponent::Set(const Transform& transform)
	{
		physx::PxTransform pxTransform(
			transform.Position.x, transform.Position.y, transform.Position.z,
			physx::PxQuat(transform.Rotation.x, transform.Rotation.y, transform.Rotation.z, transform.Rotation.w)
		);
		m_Actor->setGlobalPose(pxTransform);
	}

	void PhysicComponent::AddImpulse(const glm::vec3& dir, float force) const
	{
		physx::PxVec3 tmpDir{dir.x, dir.y, dir.z};
		static_cast<physx::PxRigidDynamic*>(m_Actor)->addForce(tmpDir * force, physx::PxForceMode::eVELOCITY_CHANGE);
	}
}
