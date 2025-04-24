#pragma once

#include <glm/glm.hpp>
#include <entt/entt.hpp>

#include <memory>
#include <vector>

namespace Sunset
{
	using World = entt::registry;

	using Entity = entt::entity;

	class RenderObject;

	struct BaseComponent
	{
		BaseComponent() = default;
		virtual ~BaseComponent() = default;
	};

	struct TransformComponent : public BaseComponent
	{
		explicit TransformComponent(const glm::vec2& loc = glm::vec2{0.f});
		glm::vec2 location;
		glm::vec2 size;
		bool bStatic;
	};

	struct RenderObjectComponent : public BaseComponent
	{
		explicit RenderObjectComponent(RenderObject* obj)
			: data(std::unique_ptr<RenderObject>(obj))
		{ }

		std::unique_ptr<RenderObject> data;
	};

	struct PhysicComponent : public BaseComponent
	{
		PhysicComponent();
		glm::vec2 velocity;
		float speed;
		virtual void ApplyPhysic(const double deltatime, TransformComponent& transform);
	};

	struct CollisionTest
	{
		static bool Intersect(TransformComponent& A, TransformComponent& B);

		static void ResolveCollision(TransformComponent& A, TransformComponent& B, float deltatime);
	};

}
