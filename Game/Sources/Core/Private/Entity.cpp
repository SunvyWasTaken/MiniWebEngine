// Sunset inc.

#include "Entity.h"

namespace Sunset
{
	Entity::Entity(Scene* _scene, const entt::entity& id)
		: m_Id(id)
		, m_Scene(_scene)
	{
	}

	Entity::Entity(const Entity& other)
		: m_Id(other.m_Id)
		, m_Scene(other.m_Scene)
	{

	}

	Entity& Entity::operator=(const Entity& other)
	{
		if (this != &other)
		{
			m_Id = other.m_Id;
			m_Scene = other.m_Scene;
		}
		return *this;
	}

	Entity::~Entity()
	{
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
