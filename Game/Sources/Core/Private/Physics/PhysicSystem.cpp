// Sunset inc.

#include "Physics/PhysicSystem.h"

#include "physx/PxPhysicsAPI.h"

#include <iostream>

namespace
{
	physx::PxFoundation* m_Foundation;
	physx::PxPhysics* m_Physics;
	physx::PxScene* m_Scene;
	physx::PxMaterial* m_DefaultMaterial;

	physx::PxDefaultAllocator      gAllocator;
	physx::PxDefaultErrorCallback  gErrorCallback;
}

namespace Sunset
{

	PhysicSystem::PhysicSystem()
	{
		m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
		if (!m_Foundation)
		{
			std::cerr << "PxCreateFoundation instancing failed\n";
			std::abort();
		}

		std::cerr << "Physx : Foundation init success\n";

		m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, physx::PxTolerancesScale(), true /*profiling*/, nullptr /*PVD*/);
		if (!m_Physics)
		{
			std::cerr << "PxCreatePhysics instancing failed\n";
			std::abort();
		}

		std::cerr << "Physx : Physc init success" << std::endl;
	}

	PhysicSystem::~PhysicSystem()
	{
		m_Physics->release();
		m_Foundation->release();
		ENGINE_LOG_TRACE("Physx : Destroy Complet")
	}

	void PhysicSystem::Init()
	{
		physx::PxSceneDesc sceneDesc(m_Physics->getTolerancesScale());
		sceneDesc.gravity = physx::PxVec3(0, -9.81f, 0);
		sceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		m_Scene = m_Physics->createScene(sceneDesc);

		ENGINE_LOG_TRACE("Physx : Scene init success")

		m_DefaultMaterial = m_Physics->createMaterial(0.5f, 0.5f, 0.6f);
	}

	void PhysicSystem::Update(float deltatime)
	{
		m_Scene->simulate(deltatime);
		m_Scene->fetchResults(true);
	}

	void PhysicSystem::Shutdown()
	{
		m_DefaultMaterial->release();
		m_Scene->release();
	}

	void PhysicSystem::CreateShape(const PhyscShape::Type& shape)
	{
		std::visit(overloads
		{
			[&](const std::monostate& state)
			{
				
			},
			[&](const PhyscShape::Cube& cube)
			{
				glm::vec3 halfExtents = glm::vec3(1.0f, 2.0f, 0.5f);
				physx::PxBoxGeometry boxGeom(physx::PxVec3(halfExtents.x, halfExtents.y, halfExtents.z));
				
				// Create dynamic actor
				physx::PxRigidDynamic* boxActor = PxCreateDynamic(
					*m_Physics,
					physx::PxTransform(physx::PxVec3(0,5,0)), // position
					boxGeom,
					*m_DefaultMaterial,
					10.0f // masse
				);
				m_Scene->addActor(*boxActor);
			},
			[&](const PhyscShape::Sphere& sphere)
			{
				float radius = 1.5f;
				physx::PxSphereGeometry sphereGeom(radius);

				physx::PxRigidDynamic* sphereActor = PxCreateDynamic(
					*m_Physics,
					physx::PxTransform(physx::PxVec3(2, 5, 0)),
					sphereGeom,
					*m_DefaultMaterial,
					5.0f
				);
				m_Scene->addActor(*sphereActor);
			},
			[&](const PhyscShape::Plane& plane)
			{}
		}, shape);
	}

}
