// Sunset inc.

#pragma once

#include "Physics/PhysicSystem.h"
#include "Components/RenderComponent.h"
#include "Components/ScriptComponent.h"
#include "Components/TransformComponent.h"
#include "Entity.h"
#include "Scene.h"

namespace
{
	entt::registry m_Entitys;
	std::vector<entt::entity> m_EntityToDestroy;
	Sunset::PhysicSystem m_PhysicSystem;
}

namespace Sunset
{
	Scene::Scene()
	{
		m_PhysicSystem.Init();
	}

	Scene::~Scene()
	{
		m_Entitys.clear();
		m_PhysicSystem.Shutdown();
	}

	void Scene::Begin()
	{
	}

	void Scene::Update(const float deltatime)
	{
		auto scripts = m_Entitys.view<ScriptComponent>();
		for (auto&& [entity, script] : scripts.each())
		{
			script.m_FuncUpdate(entity, deltatime);
		}
	}

	void Scene::UpdatePhysic(const float deltatime)
	{
		// Update physic after inputs and before render.
		m_PhysicSystem.Update(deltatime);
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

	entt::registry& Scene::GetEntitys()
	{
		return m_Entitys;
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
