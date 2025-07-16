// Sunset inc.

#include "Components/TransformComponent.h"

#include "Application.h"
#include "Components/PhysicComponent.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Entity.h"

namespace Sunset
{
	TransformComponent::TransformComponent(const glm::vec3& position, const glm::quat& rotation)
		: m_Position(position)
		, m_Size(1.f)
		, m_Rotation(rotation)
		, m_Model(1.f)
		, bDirty(true)
	{
		ProcessModel();
	}

	TransformComponent::~TransformComponent()
	{
	}

	void TransformComponent::SetPosition(const glm::vec3& newPosition)
	{
		m_Position = newPosition;
		bDirty = true;
		OnTransformChange.Calls(m_Position);
	}

	void TransformComponent::AddPosition(const glm::vec3& direction)
	{
		m_Position += direction;
		bDirty = true;
		OnTransformChange.Calls(m_Position);
	}

	void TransformComponent::AddPitch(float value)
	{
		m_Rotation.x += value;
		bDirty = true;
	}

	void TransformComponent::AddYaw(float value)
	{
		m_Rotation.y += value;
		bDirty = true;
	}

	void TransformComponent::AddRoll(float value)
	{
		m_Rotation.z += value;
		bDirty = true;
	}

	void TransformComponent::SetRotation(const glm::vec3& rotation)
	{
		m_Rotation = glm::quat(glm::radians(rotation));;
		bDirty = true;
	}

	void TransformComponent::SetRotation(const glm::quat& rotation)
	{
		m_Rotation = rotation;
		bDirty = true;
	}

	void TransformComponent::SetSize(const glm::vec3& newSize)
	{
		m_Size = newSize;
		bDirty = true;
	}

	void TransformComponent::ProcessModel()
	{
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_Position);
		glm::mat4 rotation = glm::mat4_cast(m_Rotation);
		glm::mat4 scaling = glm::scale(glm::mat4(1.0f), m_Size);

		m_Model = translation * rotation * scaling;

		bDirty = false;
	}

	void TransformComponent::UpdatePhysic()
	{
		if (bDirty)
		{
			if (PhysicComponent* physComp = owner->GetComponent<PhysicComponent>())
			{
				physComp->Set({m_Position, m_Rotation, m_Size});
			}
		}
	}
}
