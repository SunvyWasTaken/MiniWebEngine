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
	physx::PxPvdSceneClient* pvdClient;

	physx::PxDefaultAllocator      gAllocator;
	physx::PxDefaultErrorCallback  gErrorCallback;

	template <typename T>
	physx::PxTransform GetTransform(const T& obj)
	{
		return
		{
			physx::PxVec3(obj.position.x, obj.position.y, obj.position.z),
			physx::PxQuat(obj.rotation.x, obj.rotation.y, obj.rotation.z, obj.rotation.w)
		};
	}
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
		std::cerr << "PhysX version : "
			<< ((PX_PHYSICS_VERSION >> 24) & 0xFF) << "."
			<< ((PX_PHYSICS_VERSION >> 16) & 0xFF) << "."
			<< ((PX_PHYSICS_VERSION >> 8) & 0xFF) << std::endl;

		m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, physx::PxTolerancesScale(), true, nullptr);

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
		sceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(0);
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_ACTIVE_ACTORS;
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
		m_Scene->release();
		m_DefaultMaterial->release();
	}

	physx::PxScene* PhysicSystem::GetScene() const
	{
		return m_Scene;
	}

	physx::PxRigidActor* PhysicSystem::CreateStaticShape(const PhyscShape::Type& shape)
	{
		return std::visit(overloads
		{
			[&](const std::monostate&) -> physx::PxRigidActor*
			{
				return nullptr;
			},
			[&](const PhyscShape::Cube& cube) -> physx::PxRigidActor*
			{
				physx::PxBoxGeometry boxGeom(physx::PxVec3(cube.halfExtents.x, cube.halfExtents.y, cube.halfExtents.z));
				physx::PxRigidStatic* boxActor = m_Physics->createRigidStatic(GetTransform(cube));
				physx::PxShape* shape = m_Physics->createShape(boxGeom, *m_DefaultMaterial);
				boxActor->attachShape(*shape);
				shape->release();
				m_Scene->addActor(*boxActor);
				return boxActor;
			},
			[&](const PhyscShape::Sphere& sphere) -> physx::PxRigidActor*
			{
				physx::PxSphereGeometry sphereGeom(sphere.radius);
				physx::PxRigidStatic* sphereActor = m_Physics->createRigidStatic(GetTransform(sphere));
				physx::PxShape* shape = m_Physics->createShape(sphereGeom, *m_DefaultMaterial);
				sphereActor->attachShape(*shape);
				shape->release();
				m_Scene->addActor(*sphereActor);
				return sphereActor;
			},
			[&](const PhyscShape::Plane& plane) -> physx::PxRigidActor*
			{
				physx::PxTransform pose = physx::PxTransform();

				physx::PxRigidStatic* rigidPlane = m_Physics->createRigidStatic(pose);
				physx::PxShape* shape = m_Physics->createShape(physx::PxPlaneGeometry(), *m_DefaultMaterial);
				rigidPlane->attachShape(*shape);
				shape->release();
				m_Scene->addActor(*rigidPlane);
				return rigidPlane;
			},
			[&](const PhyscShape::Capsule& capsule) -> physx::PxRigidActor*
			{
				physx::PxCapsuleGeometry capsuleGeom(capsule.radius, capsule.halfHeight);
				physx::PxTransform transform = GetTransform(capsule);

				physx::PxRigidStatic* capsuleActor = m_Physics->createRigidStatic(transform);
				physx::PxShape* shape = m_Physics->createShape(capsuleGeom, *m_DefaultMaterial);
				capsuleActor->attachShape(*shape);
				shape->release();
				m_Scene->addActor(*capsuleActor);
				return capsuleActor;
			}
		}, shape);
	}

	physx::PxRigidActor* PhysicSystem::CreateDynamicShape(const PhyscShape::Type& shape)
	{
		return std::visit(overloads
		{
			[&](const std::monostate&) -> physx::PxRigidActor*
			{
				return nullptr;
			},
			[&](const PhyscShape::Cube& cube) -> physx::PxRigidActor*
			{
				physx::PxBoxGeometry boxGeom(physx::PxVec3(cube.halfExtents.x, cube.halfExtents.y, cube.halfExtents.z));

				physx::PxRigidDynamic* boxActor = m_Physics->createRigidDynamic(GetTransform(cube));
				physx::PxShape* shapePx = m_Physics->createShape(boxGeom, *m_DefaultMaterial);
				boxActor->attachShape(*shapePx);
				shapePx->release();

				physx::PxRigidBodyExt::updateMassAndInertia(*boxActor, cube.mass);

				m_Scene->addActor(*boxActor);
				return boxActor;
			},
			[&](const PhyscShape::Sphere& sphere) -> physx::PxRigidActor*
			{
				physx::PxSphereGeometry sphereGeom(sphere.radius);

				physx::PxRigidDynamic* sphereActor = m_Physics->createRigidDynamic(GetTransform(sphere));
				physx::PxShape* shapePx = m_Physics->createShape(sphereGeom, *m_DefaultMaterial);
				sphereActor->attachShape(*shapePx);
				shapePx->release();

				physx::PxRigidBodyExt::updateMassAndInertia(*sphereActor, sphere.mass);

				m_Scene->addActor(*sphereActor);
				return sphereActor;
			},
			[&](const PhyscShape::Plane& plane) -> physx::PxRigidActor*
			{
				return nullptr;
			},
			[&](const PhyscShape::Capsule& capsule) -> physx::PxRigidActor*
			{
				physx::PxCapsuleGeometry capsuleGeom(capsule.radius, capsule.halfHeight);

				physx::PxRigidDynamic* capsuleActor = m_Physics->createRigidDynamic(GetTransform(capsule));
				capsuleActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
				capsuleActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);
				physx::PxShape* shapePx = m_Physics->createShape(capsuleGeom, *m_DefaultMaterial);
				capsuleActor->attachShape(*shapePx);
				shapePx->release();

				physx::PxRigidBodyExt::updateMassAndInertia(*capsuleActor, capsule.mass);

				m_Scene->addActor(*capsuleActor);
				return capsuleActor;
			}
		}, shape);
	}

}
