// Sunset inc.

#pragma once

#include "BaseComponent.h"

namespace Sunset
{
	namespace Shape
	{
		struct Cercle
		{
			float radius = 0.f;
		};
		using Type = std::variant<Cercle>;
	}

	class CollisionComponent : public BaseComponent
	{
		friend class CollisionSystem;
	public:
		explicit CollisionComponent(const Shape::Type& shape);
		CollisionComponent(const Shape::Type& shape, const std::function<void()>& OnCollision);
		~CollisionComponent();

		Shape::Type m_Shape;
	private:
		std::function<void()> m_OnCollision;
	};
}
