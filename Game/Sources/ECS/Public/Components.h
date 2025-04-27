#pragma once

#include "Object.h"

namespace Sunset
{

	struct BaseComponent
	{
		BaseComponent() = default;
		virtual ~BaseComponent() = default;
	};

	struct TransformComponent : public BaseComponent
	{
		explicit TransformComponent(const glm::vec2& loc = glm::vec2{ 0.f });
		glm::vec2 location;
		glm::vec2 size;
		float rot;
		bool bStatic;
	};

	namespace CollisionShape
	{
		struct Square {};
		struct Sphere {};
		using Type = std::variant<Square, Sphere>;
	};

	struct CollisionComponent : public BaseComponent
	{
		CollisionShape::Type type;
		std::vector<Entity> IgnoreActor;
		void Collision();
		std::function<void()> OnCollision;
	};

	class RenderObject;

	struct RenderObjectComponent : public BaseComponent
	{
		explicit RenderObjectComponent(RenderObject* obj);

		virtual ~RenderObjectComponent();

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
}
