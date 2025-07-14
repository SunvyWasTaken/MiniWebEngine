// Sunset inc.

#pragma once

#include "Components/CameraComponent.h"
#include "Components/PhysicComponent.h"
#include "Components/RenderComponent.h"
#include "Components/ScriptComponent.h"
#include "Components/TransformComponent.h"
#include "ComponentsUpdate.h"
#include "Entity.h"
#include "Physics/PhysicSystem.h"
#include "Scenes/Scene.h"

#ifdef _DEBUG
#include "Debug/DrawDebug.h"
#include "physx/PxPhysicsAPI.h"
#endif // _DEBUG

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

	std::vector<std::unique_ptr<Sunset::Entity>>& GetEntityList()
	{
		static std::vector<std::unique_ptr<Sunset::Entity>> m_Entitys;
		return m_Entitys;
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
		GetEntityList().clear();
	}

	Scene::~Scene()
	{
		ComponentUpdater::Clear();
		GetEntityList().clear();
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
			script.m_FuncUpdate(deltatime);
		}

		ComponentUpdater::Update(deltatime);
	}

	void Scene::PreUpdatePhysic()
	{
		auto physcEntity = GetEntities().view<TransformComponent, PhysicComponent>();
		for (auto&& [entity, transComp, physComp] : physcEntity.each())
		{
			transComp.UpdatePhysic();
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

#ifdef _DEBUG
		auto* scene = GetPhysicSystem().GetScene();
		physx::PxU32 nbActors = scene->getNbActors(physx::PxActorTypeFlag::eRIGID_STATIC | physx::PxActorTypeFlag::eRIGID_DYNAMIC);
		std::vector<physx::PxActor*> actors(nbActors);
		scene->getActors(physx::PxActorTypeFlag::eRIGID_STATIC | physx::PxActorTypeFlag::eRIGID_DYNAMIC, actors.data(), nbActors);

		for (physx::PxActor* actor : actors)
		{
			auto* rigidActor = static_cast<physx::PxRigidActor*>(actor);
			physx::PxU32 nbShapes = rigidActor->getNbShapes();
			std::vector<physx::PxShape*> shapes(nbShapes);
			rigidActor->getShapes(shapes.data(), nbShapes);

			for (physx::PxShape* shape : shapes)
			{
				physx::PxTransform globalPose = physx::PxShapeExt::getGlobalPose(*shape, *rigidActor);

				if (shape->getGeometry().getType() == physx::PxGeometryType::eBOX)
				{
					const physx::PxBoxGeometry& box = static_cast<const physx::PxBoxGeometry&>(shape->getGeometry());
					DrawDebug::get()->DrawDebugBox(globalPose, box.halfExtents);
				}
				else if (shape->getGeometry().getType() == physx::PxGeometryType::eSPHERE)
				{
					const physx::PxSphereGeometry& sphere = static_cast<const physx::PxSphereGeometry&>(shape->getGeometry());
					DrawDebug::get()->DrawDebugSphere(globalPose, sphere.radius);
				}
				else if (shape->getGeometry().getType() == physx::PxGeometryType::eCAPSULE)
				{
					const physx::PxCapsuleGeometry& capsule = static_cast<const physx::PxCapsuleGeometry&>(shape->getGeometry());
					DrawDebug::get()->DrawDebugCapsule(globalPose, capsule.radius, capsule.halfHeight);
				}
				else if (shape->getGeometry().getType() == physx::PxGeometryType::ePLANE)
				{
					DrawDebug::get()->DrawDebugPlane(globalPose);
				}
			}
		}

#endif // DEBUG

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

	void Scene::AddEntityToList(Entity* value)
	{
		GetEntityList().emplace_back(value);
	}

	void Scene::AddUpdateComponent(uint32_t id, const std::function<void(float)>& func)
	{
		ComponentUpdater::AddComponent(id, func);
	}

	void Scene::DeleteUpdateComponent(uint32_t id)
	{
		ComponentUpdater::RemoveComp(id);
	}

}
