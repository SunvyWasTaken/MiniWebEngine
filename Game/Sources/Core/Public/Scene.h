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

		Scene();

		virtual ~Scene();

		virtual void Begin();

		// Update all logic like script and inputs.
		void Update(const float deltatime);

		// Update physic after inputs and before render.
		void UpdatePhysic(const float deltatime);

		void Render();
		
		void DestroyEntity(const entt::entity& entity);

		entt::registry& GetEntitys();

		Entity CreateEntity();

	private:

		void PostUpdate();
	};
}
