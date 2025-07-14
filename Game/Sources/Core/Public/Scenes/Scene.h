// Sunset inc.

#pragma once

#include "entt/entt.hpp"

namespace Sunset
{
	class Entity;
	class Shader;

	class Scene
	{
		template <typename ...TScenes>
		friend class SceneManager;

		friend class Entity;

	public:

		Scene();

		virtual ~Scene();

		virtual void Begin();

		// Update all logic like script and inputs.
		void Update(const float deltatime);

		void PreUpdatePhysic();

		// Update physic after inputs and before render.
		void UpdatePhysic(const float deltatime);

		void Render();
		
		void DestroyEntity(const entt::entity& entity);

		entt::registry& GetEntitys();

		template <typename T>
		T* CreateEntity()
		{
			static_assert(std::is_base_of_v<Entity, T>, "The class should be derived from Entity");
			T* entity = new T(this, GetEntitys().create());
			AddEntityToList(entity);
			entity->Init();
			return entity;
		}

	private:

		void PostUpdate();

		void AddEntityToList(Entity* value);

		void AddUpdateComponent(const std::function<void(float)>& func);

		void DeleteUpdateComponent(const std::function<void(float)>& func);
	};
}
