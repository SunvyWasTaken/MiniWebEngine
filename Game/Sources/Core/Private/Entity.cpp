// Sunset inc.

#include "Entity.h"

namespace Sunset
{
	Entity::Entity(Scene* _scene, const entt::entity& id)
		: m_Id(id)
		, m_Scene(_scene)
	{
	}

	Entity::~Entity()
	{
	}

	void Entity::Destroy()
	{
		if (m_Scene)
		{
			m_Scene->DestroyEntity(m_Id);
		}
	}
}
