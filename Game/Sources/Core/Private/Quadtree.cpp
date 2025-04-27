#include "Quadtree.h"
#include "Components.h"
#include "RenderObject.h"

#define CAPACITY 1

namespace Sunset
{
	Quadtree::Quadtree()
		: loc(0.f)
		, size(0.f)
	{
	}

	Quadtree::Quadtree(const std::weak_ptr<World>& _world, const glm::vec2& _loc, const glm::vec2& _size)
		: loc(_loc)
		, size(_size)
		, world(_world)
	{
		//if (std::shared_ptr<World> tmpWorld = world.lock())
		//{
		//	Entity quad = tmpWorld->create();
		//	TransformComponent& transComp = tmpWorld->emplace<TransformComponent>(quad, loc);
		//	transComp.size = size;
		//	tmpWorld->emplace<RenderObjectComponent>(quad, new Square({2, 2}));
		//}
	}

	Quadtree::~Quadtree()
	{
	}

	void Quadtree::Update(const Entity& entity)
	{
		EntityList::iterator entt;
		if (!Contain(entity))
			Remove(entity);

		else if (!HasEntity(entity, entt))
		{
			Add(entity);
		}
		for (auto& child : childs)
		{
			if (child)
				child->Update(entity);
		}
	}

	void Quadtree::Add(const Entity& entity)
	{
		if (!Contain(entity))
			return;

		if (entitys.size() < CAPACITY)
		{
			EntityList::iterator entt;
			if (!HasEntity(entity, entt))
				entitys.emplace_back(entity);

			return;
		}

		if (!childs[0])
			Subdivide();

		for (auto& child : childs)
		{
			if (child)
				child->Add(entity);
		}
	}

	void Quadtree::Remove(const Entity& entity)
	{
		EntityList::iterator entt;
		if (HasEntity(entity, entt))
		{
			entitys.erase(entt);
		}
	}

	void Quadtree::CollideWith(const Entity& entity, EntityList& founds)
	{
		if (!Contain(entity))
			return;

		for (const auto& tmp : entitys)
		{
			if (tmp == entity)
				continue;
			if (CollisionTest::Intersect(entity, tmp, *(world.lock().get())))
			{
				founds.emplace_back(tmp);
			}
		}

		if (!(childs.size() > 0))
		{
			for (auto& child : childs)
			{
				if (child)
					child->CollideWith(entity, founds);
			}
		}
	}

	bool Quadtree::Contain(const Entity& entity)
	{
		if (std::shared_ptr<World> _world = world.lock())
		{
			TransformComponent& transComp = _world->get<TransformComponent>(entity);
			return CollisionTest::Intersect(loc, size, transComp.location, transComp.size);
		}
		return false;
	}

	bool Quadtree::HasEntity(const Entity& entity, EntityList::iterator& found)
	{
		found = std::find(entitys.begin(), entitys.end(), entity);
		return found != entitys.end();
	}

	void Quadtree::Subdivide()
	{
		float x = loc.x;
		float y = loc.y;
		float hw = size.x / 2;
		float hh = size.y / 2;
 		childs[0] = std::make_unique<Quadtree>(world, glm::vec2{x + hw, y - hh}, glm::vec2{hw, hh});
		childs[1] = std::make_unique<Quadtree>(world, glm::vec2{x - hw, y - hh}, glm::vec2{hw, hh});
		childs[2] = std::make_unique<Quadtree>(world, glm::vec2{x + hw, y + hh}, glm::vec2{hw, hh});
		childs[3] = std::make_unique<Quadtree>(world, glm::vec2{x - hw, y + hh}, glm::vec2{hw, hh});
	}
}
