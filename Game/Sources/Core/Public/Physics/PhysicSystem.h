// Sunset inc.

#pragma once

namespace physx
{
	class PxRigidActor;
	class PxRigidDynamic;
	class PxScene;
}

namespace Sunset
{
	namespace PhyscShape
	{
		using None = std::monostate; 
		struct Cube
		{
			glm::vec3 position;
			glm::quat rotation;
			glm::vec3 halfExtents;
			float mass = 1.f;
			float friction = 0.5f;
			float restitution = 0.6f;
		};
		struct Sphere
		{
			glm::vec3 position;
			glm::quat rotation;
			float radius;
			float mass = 1.0f;
			float friction = 0.5f;
			float restitution = 0.6f;
		};
		struct Plane
		{
			glm::vec3 position;
			glm::quat rotation;
		};
		struct Capsule
		{
			glm::vec3 position;
			glm::quat rotation;
			float radius;
			float halfHeight;
			float mass = 1.0f;
			float friction = 0.5f;
			float restitution = 0.6f;
		};
		using Type = std::variant<None, Cube, Sphere, Plane, Capsule>;
	}

	class PhysicSystem final
	{
	public:
		PhysicSystem();
		~PhysicSystem();

		void Init();
		void Update(float deltatime);
		void Shutdown();

		physx::PxScene* GetScene() const;

		static physx::PxRigidActor* CreateStaticShape(const PhyscShape::Type& shape);
		static physx::PxRigidActor* CreateDynamicShape(const PhyscShape::Type& shape);
	};
}
