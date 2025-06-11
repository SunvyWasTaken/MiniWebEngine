// Sunset inc.

#include "Components/CollisionComponent.h"

namespace Sunset
{
	CollisionComponent::CollisionComponent(const Shape::Type& shape)
		: m_Shape(shape)
		, m_OnCollision(nullptr)
	{
	}

	CollisionComponent::CollisionComponent(const Shape::Type& shape, const std::function<void()>& OnCollision)
		: m_Shape(shape)
		, m_OnCollision(OnCollision)
	{
	}

	CollisionComponent::~CollisionComponent()
	{
	}
}
