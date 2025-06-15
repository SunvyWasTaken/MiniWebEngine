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
		sceneDesc.gravity = physx::PxVec3(0, -1.81f, 0);
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

	physx::PxRigidActor* PhysicSystem::CreateStaticShape(const PhyscShape::Type& shape)
	{
		return std::visit(overloads{
			[&](const std::monostate&) -> physx::PxRigidActor*
			{
				return nullptr;
			},
			[&](const PhyscShape::Cube& cube) -> physx::PxRigidActor*
			{
				glm::vec3 halfExtents = cube.halfExtents;
				physx::PxBoxGeometry boxGeom(physx::PxVec3(halfExtents.x, halfExtents.y, halfExtents.z));

				glm::vec3 pos = cube.position;

				physx::PxRigidDynamic* boxActor = PxCreateDynamic(
					*m_Physics,
					physx::PxTransform(physx::PxVec3(pos.x, pos.y, pos.z)),
					boxGeom,
					*m_DefaultMaterial,
					10.0f
				);
				m_Scene->addActor(*boxActor);
				return boxActor;
			},
			[&](const PhyscShape::Sphere& sphere) -> physx::PxRigidActor*
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
				return sphereActor;
			},
			[&](const PhyscShape::Plane& plane) -> physx::PxRigidActor*
			{
				physx::PxTransform pose = physx::PxTransform(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0, 0, 1)));

				physx::PxRigidStatic* rigidPlane = m_Physics->createRigidStatic(pose);
				physx::PxShape* shape = m_Physics->createShape(physx::PxPlaneGeometry(), *m_DefaultMaterial);
				rigidPlane->attachShape(*shape);
				shape->release();

				m_Scene->addActor(*rigidPlane);
				return rigidPlane;
			}
			}, shape);
	}
}
