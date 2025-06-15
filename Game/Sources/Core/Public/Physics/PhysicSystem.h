// Sunset inc.

#pragma once

namespace Sunset
{
	namespace PhyscShape
	{
		struct Cube{};
		struct Sphere{};
		struct Plane{};
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

		static void CreateShape(const PhyscShape::Type& shape);
	};
}
