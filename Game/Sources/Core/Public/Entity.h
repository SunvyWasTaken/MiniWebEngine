// Sunset inc.

#pragma once

#include "Scene.h"
#include "Components/BaseComponent.h"

namespace Sunset
{
	class Entity
	{
	public:
		Entity(Scene* _scene = nullptr, const entt::entity& id = entt::null);

		virtual ~Entity();

		Entity(const Entity& other);

		Entity& operator=(const Entity& other);

		virtual void Init();

		void Destroy();

		operator const entt::entity& () const { return m_Id; }

		Scene* GetScene() const { return m_Scene; }

		template <typename T, typename ...Args>
		void AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of_v<BaseComponent, T>, "The class should be a child of BaseComponent");
			if (m_Scene)
			{
				T& comp = m_Scene->GetEntitys().emplace<T>(m_Id, std::forward<Args>(args)...);
				comp.owner = this;
			}
		}

		template <typename T>
		T* GetComponent() const
		{
			if(!m_Scene)
				return nullptr;

			return m_Scene->GetEntitys().try_get<T>(m_Id);
		}

		int Id() const { return static_cast<int>(m_Id); }

	private:
		entt::entity m_Id;
		Scene* m_Scene;
	};
}

#define GENERATED_BODY(name) using Sunset::Entity::Entity; using Super = Sunset::Entity; \
		name(const name& other) \
			: Super(other) \
		{} \
		name& operator=(const name& other) { if (this != &other) {Super::operator=(other);} return *this; }
