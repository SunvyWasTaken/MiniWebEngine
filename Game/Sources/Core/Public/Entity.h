// Sunset inc.

#pragma once

#include "Scenes/Scene.h"
#include "Components/BaseComponent.h"

namespace Sunset
{
	class Entity
	{
		CALLBACKS_MULTI(OnDestroy);

		template <typename, typename = void>
		struct has_Update : std::false_type {};

		template <typename T>
		struct has_Update<T, std::void_t<decltype(std::declval<T>().Update(std::declval<float>()))>> : std::true_type {};

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
		T* AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of_v<BaseComponent, T>, "The class should be a child of BaseComponent");
			if (m_Scene)
			{
				T& comp = m_Scene->GetEntitys().emplace<T>(m_Id, std::forward<Args>(args)...);
				if constexpr (has_Update<T>::value)
				{
					const std::function<void(float)>& func = [&](float dt)
						{
							comp.Update(dt);
						};

					m_Scene->AddUpdateComponent(static_cast<uint32_t>(m_Id), func);

					OnEntityDestroy.Bind([&]()
					{
						m_Scene->DeleteUpdateComponent(static_cast<uint32_t>(m_Id));
					});
				}
				comp.owner = this;
				return &comp;
			}
			return nullptr;
		}

		template <typename T>
		T* GetComponent() const
		{
			if(!m_Scene)
				return nullptr;

			return m_Scene->GetEntitys().try_get<T>(m_Id);
		}

		int Id() const { return static_cast<int>(m_Id); }

		OnDestroy OnEntityDestroy;

	private:
		entt::entity m_Id;
		Scene* m_Scene;
	};
}

#define GENERATED_BODY(name) \
		using Sunset::Entity::Entity; \
		using Super = Sunset::Entity;
