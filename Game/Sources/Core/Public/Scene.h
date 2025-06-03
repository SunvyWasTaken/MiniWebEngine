// Sunset inc.

#pragma once

#include "entt/entt.hpp"

namespace Sunset
{
	class Entity;

	class Scene
	{
		template <typename ...TScenes>
		friend class SceneManager;

	public:

		Scene();

		virtual ~Scene();

		virtual void Update(const float deltatime);
		
		void DestroyEntity(const entt::entity& entity);

		entt::registry& GetEntitys() { return m_Entitys; }

		Entity CreateEntity();

	private:

		void PostUpdate();

	private:
		entt::registry m_Entitys;

		std::vector<entt::entity> m_EntityToDestroy;
	};
}
