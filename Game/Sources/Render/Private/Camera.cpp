// Sunset inc.

#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Sunset
{
	Camera::Camera()
		: m_Position(0)
		, m_Forward(0, 0, 1)
		, m_UpVector(1, 0, 0)
		, m_Resolution(1280, 720)
		, m_Fov(45.f)
		, m_RenderDistance(2e7)
	{
	}

	void Camera::AddPosition(const glm::vec3& position)
	{
		m_Position += position;
	}

	glm::mat4 Camera::GetView() const
	{
		return glm::lookAt(m_Position + m_Forward, m_Position, m_UpVector);
	}

	glm::mat4 Camera::GetProjection() const
	{
		return glm::perspective(glm::radians(m_Fov), m_Resolution.x / m_Resolution.y, 0.1f, m_RenderDistance);
	}

}
