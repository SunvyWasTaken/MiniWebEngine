// Sunset inc.

#pragma once

namespace entt
{
	enum class entity : uint32_t;

	template <typename Entity, typename Allocator>
	class basic_registry;

	using registry = basic_registry<entity, std::allocator<entity>>;
}

namespace Sunset
{
	class CollisionSystem
	{
	public:
		CollisionSystem();

		void Update(entt::registry& entitys, const float deltatime);
	};
}
