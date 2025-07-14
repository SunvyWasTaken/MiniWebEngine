// Sunset inc.

#pragma once

#include "BaseComponent.h"
#include "Physics/PhysicSystem.h"

namespace physx
{
	class PxRigidActor;
}

namespace Sunset
{
	enum class Physc
	{
		eForce,
		eImpusle,
		eSet,
		eAcc
	};

	class PhysicComponent : public BaseComponent
	{
	public:
		explicit PhysicComponent(const PhyscShape::Type& shape, bool IsStatic = true);
		~PhysicComponent();
		glm::vec3 GetPosition() const;
		glm::quat GetRotation() const;

		void Set(const Transform& transform);

		void AddForce(const glm::vec3& dir, const Physc& type = Physc::eForce, float force = 1.f) const;

		glm::vec3 GetVelocity() const;

		bool IsFalling() const;

	private:
		PhyscShape::Type m_Shape;
		physx::PxRigidActor* m_Actor;
	};
}
