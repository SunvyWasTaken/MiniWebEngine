#pragma once

#include "entt/entt.hpp"

namespace Sunset
{
	using World = entt::registry;

	using Entity = entt::entity;

	struct CollisionTest
	{
		static bool Intersect(const Entity& A, const Entity& B, World& world);
		static bool Intersect(const glm::vec2& centerA, const glm::vec2& sizeA, const glm::vec2& centerB, const glm::vec2& sizeB);

		static void ResolveCollision(Entity& A, Entity& B, World& world, float deltatime);
	};

}
