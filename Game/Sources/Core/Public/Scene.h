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

	public:

		Scene() = default;

		virtual ~Scene();

		virtual void Begin() {};

		virtual void Update(const float deltatime);

		void Render(Shader* shader);
		
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
