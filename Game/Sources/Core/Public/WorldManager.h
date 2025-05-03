#pragma once

#include "Object.h"

namespace Sunset
{
	class Render;

	template <typename ...Ws>
	class WorldManager
	{
		using Worlds = std::variant<std::monostate, Ws...>;

		struct UpdateVisitor
		{
			double dt;
			template <typename Wo>
			void operator()(Wo& w) const
			{
				w.Update(dt);
			}
			void operator()(std::monostate&) const
			{ }
		};

		struct RenderVisitor
		{
			Render* rw = nullptr;
			template<typename Wo>
			void operator()(Wo& w) const {
				w.RenderObjs(*rw);
			}

			void operator()(std::monostate&) const {
				// Rien à faire
			}
		};

	public:
		template <typename NewWorld, typename ...Args>
		void LoadWorld(Args&&... args)
		{
			static_assert(std::is_base_of_v<World, NewWorld>, "Loading world is not base of world");
			m_World.emplace<NewWorld>(std::forward<Args>(args)...);
		}

		void Update(double deltatime)
		{
			std::visit(UpdateVisitor{deltatime}, m_World);
		}

		void RenderObj(Render* rw)
		{
			std::visit(RenderVisitor{rw}, m_World);
		}

	private:
		Worlds m_World;
	};
}
