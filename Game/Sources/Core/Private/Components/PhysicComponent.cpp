// Sunset inc.

#include "Components/PhysicComponent.h"

#include "physx/PxPhysicsAPI.h"

namespace
{
	physx::PxForceMode::Enum ToPxForceMode(const Sunset::Physc& mode)
	{
		switch (mode)
		{
		case Sunset::Physc::eForce:    return physx::PxForceMode::eFORCE;
		case Sunset::Physc::eImpusle:  return physx::PxForceMode::eIMPULSE;
		case Sunset::Physc::eSet:      return physx::PxForceMode::eVELOCITY_CHANGE;
		case Sunset::Physc::eAcc:      return physx::PxForceMode::eACCELERATION;
		default:               return physx::PxForceMode::eFORCE;
		}
	}

}

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

	void PhysicComponent::AddForce(const glm::vec3& dir, const Physc& type /* = Physc::eForce*/, float force /* = 1.f */) const
	{
		physx::PxVec3 tmpDir{dir.x, dir.y, dir.z};
		static_cast<physx::PxRigidDynamic*>(m_Actor)->addForce(tmpDir * force, ToPxForceMode(type));
	}

	glm::vec3 PhysicComponent::GetVelocity() const
	{
		physx::PxVec3 vel = static_cast<physx::PxRigidDynamic*>(m_Actor)->getLinearVelocity();
		return {vel.x , vel.y, vel.z};
	}

	bool PhysicComponent::IsFalling() const
	{
		return !(GetVelocity().z != 0.f);
	}
}
