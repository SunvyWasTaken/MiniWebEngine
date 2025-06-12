// Sunset inc.

#pragma once

#include "CollisionSystem.h"
#include "Components/RenderComponent.h"
#include "Components/ScriptComponent.h"
#include "Components/TransformComponent.h"
#include "Entity.h"
#include "Scene.h"

namespace
{
	Sunset::CollisionSystem collisionSys;
}

namespace Sunset
{
	Scene::~Scene()
	{
		m_Entitys.clear();
	}

	void Scene::Update(const float deltatime)
	{
		auto scripts = m_Entitys.view<ScriptComponent>();
		for (auto&& [entity, script] : scripts.each())
		{
			script.m_FuncUpdate(entity, deltatime);
		}

		collisionSys.Update(m_Entitys, deltatime);
	}

	void Scene::Render()
	{
		auto rendable = m_Entitys.view<TransformComponent, RenderComponent>();
		for (auto&& [entity, transComp, rendComp] : rendable.each())
		{
			rendComp(transComp.GetModel());
		}
	}

	void Scene::DestroyEntity(const entt::entity& entity)
	{
		auto it = std::find(m_EntityToDestroy.begin(), m_EntityToDestroy.end(), entity);
		if (it == m_EntityToDestroy.end())
		{
			m_EntityToDestroy.emplace_back(entity);
		}
	}

	Entity Scene::CreateEntity()
	{
		Entity entity{this, m_Entitys.create()};
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
