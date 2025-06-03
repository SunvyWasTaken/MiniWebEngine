// Sunset inc.

#pragma once

#include "Entity.h"
#include "Scene.h"

namespace Sunset
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
		m_Entitys.clear();
	}

	void Scene::Update(const float deltatime)
	{
	}

	void Scene::DestroyEntity(const entt::entity& entity)
	{
		m_EntityToDestroy.emplace_back(entity);
	}

	Entity Scene::CreateEntity()
	{
		Entity entity{this};
		return entity;
	}

	void Scene::PostUpdate()
	{
		for (entt::entity& entity : m_EntityToDestroy)
		{
			m_Entitys.destroy(entity);
		}
		m_EntityToDestroy.clear();
	}

}
