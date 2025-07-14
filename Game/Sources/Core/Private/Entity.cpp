// Sunset inc.

#include "Entity.h"

namespace Sunset
{
	Entity::Entity(Scene* _scene, const entt::entity& id)
		: m_Id(id)
		, m_Scene(_scene)
	{
		ENGINE_LOG_INFO("Entity Create")
	}

	Entity::Entity(const Entity& other)
		: m_Id(other.m_Id)
		, m_Scene(other.m_Scene)
	{
		ENGINE_LOG_INFO("Entity Create by Copy")

	}

	Entity& Entity::operator=(const Entity& other)
	{
		ENGINE_LOG_INFO("Entity Create Copy assignation")
		if (this != &other)
		{
			m_Id = other.m_Id;
			m_Scene = other.m_Scene;
		}
		return *this;
	}

	Entity::~Entity()
	{
		ENGINE_LOG_INFO("Entity destroy")
		OnEntityDestroy.Calls();
	}

	void Entity::Init()
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
