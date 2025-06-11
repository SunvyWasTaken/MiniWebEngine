// Sunset inc.

#include "CollisionSystem.h"
#include "Components/TransformComponent.h"
#include "Components/CollisionComponent.h"

#include "entt/entt.hpp"

namespace
{
	bool IsColliding(const Sunset::CollisionComponent& c1, const Sunset::TransformComponent& t1, const Sunset::CollisionComponent& c2, const Sunset::TransformComponent& t2)
	{
		return std::visit(overloads{
			[&](Sunset::Shape::Cercle shape1, Sunset::Shape::Cercle shape2)
			{
				float dist = glm::length(t1.GetPosition() - t2.GetPosition());
				float combinedRadius = shape1.radius + shape1.radius;
				return dist <= combinedRadius;
			}
		}, c1.m_Shape, c2.m_Shape);
	}

	void ResolveCollision(const Sunset::CollisionComponent& c1, Sunset::TransformComponent& t1, const Sunset::CollisionComponent& c2, Sunset::TransformComponent& t2)
	{
		std::visit(overloads{
			[&](Sunset::Shape::Cercle shape1, Sunset::Shape::Cercle shape2)
			{
				glm::vec3 delta = t2.GetPosition() - t1.GetPosition();
				float distance = glm::length(delta);
				float sumRadii = shape1.radius + shape2.radius;
				if (distance < sumRadii)
				{
					float overlap = sumRadii - distance;
					glm::vec3 resolutionDir = glm::normalize(delta);
					glm::vec3 resolution = resolutionDir * overlap;

					t1.AddPosition(-resolution * 0.5f);
					t2.AddPosition(resolution * 0.5f);
				}
			}
		}, c1.m_Shape, c2.m_Shape);
	}
}

namespace Sunset
{
	CollisionSystem::CollisionSystem()
	{
	}

	void CollisionSystem::Update(entt::registry& entitys, const float deltatime)
	{
		auto colliders = entitys.view<TransformComponent, CollisionComponent>();
		for (auto&& [ent, transComp, collComp] : colliders.each())
		{
			for (auto&& [ent2, transComp2, collComp2] : colliders.each())
			{
				if (ent == ent2)
					continue;

				if (IsColliding(collComp, transComp, collComp2, transComp2))
				{
					ResolveCollision(collComp, transComp, collComp2, transComp2);
					if (collComp.m_OnCollision)
						collComp.m_OnCollision();
					if (collComp2.m_OnCollision)
						collComp2.m_OnCollision();
				}
			}
		}
	}
}
