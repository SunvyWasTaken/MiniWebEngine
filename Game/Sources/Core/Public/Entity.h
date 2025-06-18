// Sunset inc.

#pragma once

#include "Scene.h"
#include "Components/BaseComponent.h"

namespace Sunset
{
	class Entity
	{
	public:
		Entity(Scene* _scene, const entt::entity& id);

		virtual ~Entity();

		virtual void Init();

		void Destroy();

		operator const entt::entity& () const { return m_Id; }

		Scene* GetScene() const { return m_Scene; }

		template <typename T, typename ...Args>
		void AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of_v<BaseComponent, T>, "The class should be a child of BaseComponent");
			if (m_Scene)
				m_Scene->GetEntitys().emplace<T>(m_Id, std::forward<Args>(args)...);
		}

		template <typename T>
		T* GetComponent()
		{
			if(!m_Scene)
				return nullptr;

			return &m_Scene->GetEntitys().get<T>(m_Id);
		}

	private:
		entt::entity m_Id;
		Scene* m_Scene;
	};
}

#define GENERATED_BODY using Sunset::Entity::Entity; using Super = Sunset::Entity;
