#include "Object.h"
#include "Components.h"
#include "Render.h"

namespace Sunset
{
	World::World()
	{

	}

	World::~World()
	{
		id.clear();
	}

	void World::Update(double deltatime)
	{
		for (auto& tmp : entitys)
		{
			tmp->Update(deltatime);
		}
	}

	void World::RenderObjs(Render& window)
	{
		auto rendables = id.view<Sunset::TransformComponent, Sunset::RenderObjectComponent>();
		rendables.each([&](Sunset::TransformComponent& transform, Sunset::RenderObjectComponent& RenderObj)
			{
				window.RenderObj(transform, *(RenderObj.data));
			});
	}

	void World::PostRenderObjs()
	{
	}

	void World::DestroyEntity(Entity* entity)
	{
		id.destroy(entity->id);

		auto it = std::find_if(entitys.begin(), entitys.end(),
			[&entity](const std::unique_ptr<Entity>& e) {
				return e->id == entity->id;
			});
		if (it != entitys.end()) {
			entitys.erase(it);
		}
	}

	Entity::Entity(World* _world)
		: world(_world)
		, id(entt::null)
	{
		if (world)
			id = world->id.create();
	}

	Entity::~Entity()
	{
	}

	void Entity::Begin()
	{
	}

	void Entity::Update(double deltatime)
	{
		//if (lifetime != 0.f)
		//{
		//	currentLifetime += deltatime;
		//	if (currentLifetime >= lifetime)
		//	{
		//		Destroy();
		//	}
		//}
	}

	void Entity::Destroy()
	{
		world->DestroyEntity(this);
	}

	World* Entity::GetWorld() const
	{
		return world;
	}

	bool CollisionTest::Intersect(const Entity& A, const Entity& B, World& world)
	{
		auto [transCompA, colliCompA] = world->try_get<TransformComponent, CollisionComponent>(A);
		auto [transCompB, colliCompB] = world->try_get<TransformComponent, CollisionComponent>(B);
		if (!transCompA || !colliCompA || !transCompB || !colliCompB)
			return false;

		return std::visit(overload{
			[&](const CollisionShape::Square& a, const CollisionShape::Square& b)
			{
				glm::vec2 aMin = transCompA->location - transCompA->size * 0.5f;
				glm::vec2 aMax = transCompA->location + transCompA->size * 0.5f;

				glm::vec2 bMin = transCompB->location - transCompB->size * 0.5f;
				glm::vec2 bMax = transCompB->location + transCompB->size * 0.5f;

				bool overlapX = aMin.x <= bMax.x && aMax.x >= bMin.x;
				bool overlapY = aMin.y <= bMax.y && aMax.y >= bMin.y;

				return overlapX && overlapY;
			},
			[&](const CollisionShape::Square& a, const CollisionShape::Sphere& b)
			{
				return false;
			},
			[&](const CollisionShape::Sphere& a, const CollisionShape::Square& b)
			{
				return false;
			},
			[&](const CollisionShape::Sphere& a, const CollisionShape::Sphere& b)
			{
				return false;
			}
		}, colliCompA->type, colliCompB->type);
	}

	bool CollisionTest::Intersect(const glm::vec2& centerA, const glm::vec2& sizeA, const glm::vec2& centerB, const glm::vec2& sizeB)
	{
		glm::vec2 aMin = centerA - sizeA * 0.5f;
		glm::vec2 aMax = centerA + sizeA * 0.5f;

		glm::vec2 bMin = centerB - sizeB * 0.5f;
		glm::vec2 bMax = centerB + sizeB * 0.5f;

		bool overlapX = aMin.x <= bMax.x && aMax.x >= bMin.x;
		bool overlapY = aMin.y <= bMax.y && aMax.y >= bMin.y;

		return overlapX && overlapY;
	}

	void CollisionTest::ResolveCollision(Entity& A, Entity& B, World& world, float deltatime)
	{
		TransformComponent& transCompA = (*world).get<TransformComponent>(A);
		TransformComponent& transCompB = (*world).get<TransformComponent>(B);
		glm::vec2 delta = transCompB.location - transCompA.location;
		glm::vec2 overlap = (transCompA.size + transCompB.size) * 0.5f - glm::abs(delta);

		if (overlap.x < overlap.y)
		{
			float direction = delta.x < 0 ? -1.0f : 1.0f;
			if (!transCompA.bStatic)
				transCompA.location.x -= direction * overlap.x * 0.5f;
			if (!transCompB.bStatic)
				transCompB.location.x += direction * overlap.x * 0.5f;
		}
		else
		{
			float direction = delta.y < 0 ? -1.0f : 1.0f;
			if (!transCompA.bStatic)
				transCompA.location.y -= direction * overlap.y * 0.5f;
			if (!transCompB.bStatic)
				transCompB.location.y += direction * overlap.y * 0.5f;
		}
	}

	void CollisionComponent::Collision()
	{
		if (OnCollision)
		{
			OnCollision();
		}
	}
}
