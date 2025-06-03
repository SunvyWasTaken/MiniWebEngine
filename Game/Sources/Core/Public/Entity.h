// Sunset inc.

#pragma once

#include "Scene.h"

namespace Sunset
{
	class Entity
	{
	public:
		explicit Entity(Scene* _scene = nullptr);

		virtual ~Entity();

		operator entt::entity& () { return id; }

		template <typename T, typename ...Args>
		void AddComponent(Args&&... args)
		{
			if (m_Scene)
				m_Scene->GetEntitys().emplace<T>(id, std::forward<Args>(args)...);
		}

		template <typename T>
		T* GetComponent()
		{
			if(!m_Scene)
				return nullptr;

			return &m_Scene->GetEntitys().get<T>(id);
		}

	private:
		entt::entity id;
		Scene* m_Scene;
	};
}
