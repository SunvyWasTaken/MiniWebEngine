// Sunset inc.

#pragma once

#include "Physics/PhysicSystem.h"
#include "Components/RenderComponent.h"
#include "Components/ScriptComponent.h"
#include "Components/TransformComponent.h"
#include "Components/PhysicComponent.h"
#include "Entity.h"
#include "Scene.h"

namespace
{
	entt::registry& GetEntities()
	{
		static entt::registry m_Entities;
		return m_Entities;
	}

	std::vector<entt::entity>& GetEntitiesToDestroy()
	{
		static std::vector<entt::entity> m_EntityToDestroy;
		return m_EntityToDestroy;
	}

	Sunset::PhysicSystem& GetPhysicSystem()
	{
		static Sunset::PhysicSystem m_PhysicSystem;
		return m_PhysicSystem;
	}
}

namespace Sunset
{
	Scene::Scene()
	{
		GetPhysicSystem().Init();
	}

	Scene::~Scene()
	{
		GetEntities().clear();
		GetPhysicSystem().Shutdown();
	}

	void Scene::Begin()
	{
	}

	void Scene::Update(const float deltatime)
	{
		auto scripts = GetEntities().view<ScriptComponent>();
		for (auto&& [entity, script] : scripts.each())
		{
			script.m_FuncUpdate(entity, deltatime);
		}
	}

	void Scene::UpdatePhysic(const float deltatime)
	{
		// Update physic after inputs and before render.
		GetPhysicSystem().Update(deltatime);

		auto physcEntity = GetEntities().view<TransformComponent, PhysicComponent>();
		for (auto&& [entity, transComp, physComp] : physcEntity.each())
		{
			transComp.SetPosition(physComp.GetPosition());
			transComp.SetRotation(physComp.GetRotation());
		}
	}

	void Scene::Render()
	{
		auto rendable = GetEntities().view<TransformComponent, RenderComponent>();
		for (auto&& [entity, transComp, rendComp] : rendable.each())
		{
			rendComp(transComp.GetModel());
		}
	}

	void Scene::DestroyEntity(const entt::entity& entity)
	{
		auto it = std::find(GetEntitiesToDestroy().begin(), GetEntitiesToDestroy().end(), entity);
		if (it == GetEntitiesToDestroy().end())
		{
			GetEntitiesToDestroy().emplace_back(entity);
		}
	}

	entt::registry& Scene::GetEntitys()
	{
		return GetEntities();
	}

	void Scene::PostUpdate()
	{
		for (entt::entity& entity : GetEntitiesToDestroy())
		{
			GetEntities().destroy(entity);
		}
		GetEntitiesToDestroy().clear();
	}

}
