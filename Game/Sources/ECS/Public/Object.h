#pragma once

#include "entt/entt.hpp"

namespace Sunset
{
	class Render;

	class World
	{
		friend class Entity;
		using Id = entt::registry;
	public:
		World();
		virtual ~World();

		virtual void Update(double deltatime);

		Id* operator->()
		{
			return &id;
		}

		Id& operator*()
		{
			return id;
		}

		template <typename T>
		T* CreateEntity();

		void RenderObjs(Render& window);

		virtual void PostRenderObjs();

		void DestroyEntity(Entity* entity);

	private:
		Id id;
		std::vector<std::unique_ptr<Entity>> entitys;
	};

	class Entity
	{
		friend World;
		using Id = entt::entity;
	public:
		Entity() = delete;
		explicit Entity(World* _world = nullptr);

		virtual ~Entity();

		operator Id() const
		{
			return id;
		}

		virtual void Begin();

		virtual void Update(double deltatime);

		template <typename T, typename ...Args>
		T& AddComponent(Args&&... args)
		{
			return world->id.emplace<T>(id, std::forward<Args>(args)...);
		}

		template <typename T>
		T& GetComponent()
		{
			return world->id.get<T>(id);
		}

		void Destroy();

	protected:
		
		float lifetime = 0.f;

	private:
		World* world;
		Id id;
		float currentLifetime = 0.f;
	};

	template <typename T>
	T* World::CreateEntity()
	{
		static_assert(std::is_base_of_v<Entity, T>, "Create entity doit hériter de entity");
		T* tmp = new T(this);
		tmp->Begin();
		entitys.emplace_back(std::unique_ptr<Entity>(tmp));
		return tmp;
	};


	struct CollisionTest
	{
		static bool Intersect(const Entity& A, const Entity& B, World& world);
		static bool Intersect(const glm::vec2& centerA, const glm::vec2& sizeA, const glm::vec2& centerB, const glm::vec2& sizeB);

		static void ResolveCollision(Entity& A, Entity& B, World& world, float deltatime);
	};

}
