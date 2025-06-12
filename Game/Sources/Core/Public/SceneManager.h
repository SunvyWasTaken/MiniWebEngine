// Sunset inc.

#pragma once

#include "Shaders.h"
#include "Scene.h"

namespace Sunset
{
	template <typename ...TScenes>
	class SceneManager
	{
		using Scenes = std::variant<TScenes...>;
	public:

		SceneManager()
		{
			ENGINE_LOG_TRACE("Scene manager init")
			m_Scenes.emplace<std::variant_alternative_t<0, Scenes>>();
			m_CurrentScene = std::get_if<std::variant_alternative_t<0, Scenes>>(&m_Scenes);
		}

		void Update(const float deltatime)
		{
			std::visit([&](auto&& scene)
			{
				scene.Update(deltatime);
				scene.PostUpdate();
			}, m_Scenes);

			PostUpdate();
		}

		void Render()
		{
			std::visit([&](auto&& scene)
				{
					scene.Render();
				}, m_Scenes);
		}

		template <typename S>
		void LoadScene()
		{
			static_assert((std::is_same_v<S, TScenes> || ...), "The scene isn't valid");
			Impl_LoadScene = [this]()
			{
				ENGINE_LOG_TRACE("Start Loading scene : {}.", typeid(S).name())
				m_Scenes.emplace<S>();
				m_CurrentScene = std::get_if<S>(&m_Scenes);
				m_CurrentScene->Begin();
			};
		}

		Scene* GetScene()
		{
			return m_CurrentScene;
		}

	private:
		
		void PostUpdate()
		{
			if (Impl_LoadScene)
				Impl_LoadScene();

			Impl_LoadScene = nullptr;
		}

	private:

		Scenes m_Scenes;

		Scene* m_CurrentScene = nullptr;

		std::function<void()> Impl_LoadScene = nullptr;
	};
}
