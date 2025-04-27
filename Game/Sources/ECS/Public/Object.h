#pragma once

#include "entt/entt.hpp"

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
		float rot;
		bool bStatic;
	};

	namespace CollisionShape
	{
		struct Square{};
		struct Sphere{};
		using Type = std::variant<Square, Sphere>;
	};

	struct CollisionComponent : public BaseComponent
	{
		CollisionShape::Type type;
		std::vector<Entity> IgnoreActor;
		void Collision();
		std::function<void()> OnCollision;
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
		virtual ~PhysicComponent();
		virtual void ApplyPhysic(const double deltatime, TransformComponent& transform);
		void AddImpulse(const glm::vec2& dir, float force);
		glm::vec2 velocity;
		float maxSpeed;
		float speed;
	};

	struct CollisionTest
	{
		static bool Intersect(const Entity& A, const Entity& B, World& world);

		static void ResolveCollision(TransformComponent& A, TransformComponent& B, float deltatime);
	};

}
