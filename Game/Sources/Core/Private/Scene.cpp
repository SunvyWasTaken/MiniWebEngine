// Sunset inc.

#pragma once

#include "Components/RenderComponent.h"
#include "Components/TransformComponent.h"
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

	void Scene::Render(Shader* shader)
	{
		auto rendable = m_Entitys.view<TransformComponent, RenderComponent>();
		for (auto&& [entity, transComp, rendComp] : rendable.each())
		{
			rendComp(shader, transComp.GetModel());
		}
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
