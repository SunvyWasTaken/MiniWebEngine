// Sunset inc.

#pragma once

namespace Sunset
{
	template <typename ...TScenes>
	class SceneManager
	{
		using Scenes = std::variant<TScenes...>;
	public:

		void Update(const float deltatime)
		{
			std::visit([&](auto&& scene)
			{
				scene.Update(deltatime);
				scene.PostUpdate();
			}, m_Scenes);

			PostUpdate();
		}

		template <typename S>
		void LoadScene()
		{
			static_assert((std::is_same_v<S, TScenes>, ...), "The scene isn't valid");
			Impl_LoadScene = [this](){ m_Scenes.emplace<S>(); };
		}

	private:
		
		void PostUpdate()
		{
			if (Impl_LoadScene)
			{
				Impl_LoadScene();
			}
			Impl_LoadScene = nullptr;
		}

	private:

		Scenes m_Scenes;

		std::function<void()> Impl_LoadScene = nullptr;
	};
}
