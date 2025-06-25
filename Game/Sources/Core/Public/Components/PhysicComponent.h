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
	class PhysicComponent : public BaseComponent
	{
	public:
		explicit PhysicComponent(const PhyscShape::Type& shape, bool IsStatic = true);
		~PhysicComponent();
		glm::vec3 GetPosition() const;
		glm::quat GetRotation() const;

		void Set(const Transform& transform);

	private:
		PhyscShape::Type m_Shape;
		physx::PxRigidActor* m_Actor;
	};
}
