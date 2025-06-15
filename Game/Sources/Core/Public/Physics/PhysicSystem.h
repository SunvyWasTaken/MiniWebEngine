// Sunset inc.

#pragma once

namespace physx
{
	class PxRigidActor;
	class PxRigidDynamic;
}

namespace Sunset
{
	namespace PhyscShape
	{
		struct Cube
		{
			glm::vec3 position;
			glm::vec3 halfExtents;
		};
		struct Sphere
		{
			glm::vec3 position;
			float radius;
		};
		struct Plane
		{
			glm::vec3 position;
		};
		using Type = std::variant<std::monostate, Cube, Sphere, Plane>;
	}

	class PhysicSystem final
	{
	public:
		PhysicSystem();
		~PhysicSystem();

		void Init();
		void Update(float deltatime);
		void Shutdown();

		static physx::PxRigidActor* CreateStaticShape(const PhyscShape::Type& shape);
	};
}
