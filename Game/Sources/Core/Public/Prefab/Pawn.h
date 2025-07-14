// Sunset inc.

#pragma once

#include "Entity.h"

namespace Sunset
{
	class TransformComponent;
	class PhysicComponent;
	class CameraComponent;

	class Pawn : public Entity
	{
	public:
		GENERATED_BODY(Pawn);

		void Init() override;

		void Update(float deltatime);

		void AddPosition(const glm::vec3& dir);

		glm::vec3 GetPosition() const;

		void Jump();

	private:
		TransformComponent* m_TransComp = nullptr;
		PhysicComponent* m_PhysComp = nullptr;
		CameraComponent* m_CameraComp = nullptr;
	};
}