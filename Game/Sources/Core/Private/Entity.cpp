// Sunset inc.

#include "Entity.h"

namespace Sunset
{
	Entity::Entity(Scene* _scene, const entt::entity& id)
		: m_Id(id)
		, m_Scene(_scene)
	{
		ENGINE_LOG_TRACE("Entity Create {}", static_cast<uint32_t>(id));
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
