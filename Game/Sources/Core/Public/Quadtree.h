#pragma once

#include "Object.h"

namespace Sunset
{
	class Quadtree final
	{
		using EntityList = std::vector<Entity>;
		using ChildList = std::array<std::unique_ptr<Quadtree>, 4>;

	public:
		Quadtree();
		Quadtree(const std::weak_ptr<World>& _world, const glm::vec2& _loc, const glm::vec2& _size);
		~Quadtree();

		void Update(const Entity& entity);
		void Add(const Entity& entity);
		void Remove(const Entity& entity);
		void CollideWith(const Entity& entity, EntityList& founds);

	private:

		bool Contain(const Entity& entity);
		bool HasEntity(const Entity& entity, EntityList::iterator& found);
		void Subdivide();

	private:

		glm::vec2 loc, size;
		EntityList entitys;
		ChildList childs;
		std::weak_ptr<World> world;
	};
}

