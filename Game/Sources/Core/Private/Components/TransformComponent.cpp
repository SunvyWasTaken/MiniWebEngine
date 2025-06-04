// Sunset inc.

#include "Components/TransformComponent.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Sunset
{
	TransformComponent::TransformComponent(const glm::vec3& position)
		: m_Position(position)
		, m_Model(1.f)
	{
		ProcessModel();
		ENGINE_LOG_TRACE("Transcomp create")
	}

	TransformComponent::~TransformComponent()
	{
	}

	glm::vec3 TransformComponent::GetPosition() const
	{
		return m_Position;
	}

	void TransformComponent::SetPosition(const glm::vec3& newPosition)
	{
		m_Position = newPosition;
		ProcessModel();
	}

	void TransformComponent::AddPosition(const glm::vec3& direction)
	{
		m_Position += direction;
		ProcessModel();
	}

	void TransformComponent::ProcessModel()
	{
		m_Model = glm::mat4(1.f);
		m_Model = glm::translate(m_Model, m_Position);
	}
}
