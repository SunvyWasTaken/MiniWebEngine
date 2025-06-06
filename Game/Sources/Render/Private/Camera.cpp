// Sunset inc.

#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Sunset
{
	Camera::Camera()
		: m_Position(0)
		, m_Forward(0)
		, m_UpVector(0)
		, m_Resolution(1280, 720)
		, m_Rotation(-90.f, 0.f, 0.f)
		, m_Fov(45.f)
		, m_RenderDistance(2e7)
		, m_Projection(Projection::Perspective)
	{
		ProcessVector();
	}

	void Camera::AddPosition(const glm::vec3& position)
	{
		m_Position += position;
		ProcessVector();
	}

	glm::mat4 Camera::GetView() const
	{
		return glm::lookAt(m_Position, m_Position + m_Forward, m_UpVector);
	}

	glm::mat4 Camera::GetProjection() const
	{
		glm::mat4 pro(1.f);
		switch (m_Projection)
		{
		case Projection::Perspective:
			pro = glm::perspective(glm::radians(m_Fov), m_Resolution.x / m_Resolution.y, 0.1f, m_RenderDistance);
			break;
		case Projection::Orthographique:
			pro = glm::ortho(720.f, 0.f, 1280.f, 0.f, 0.1f, m_RenderDistance);
			break;
		}
		return pro;
	}

	void Camera::SetRotation(const glm::vec3& rotation)
	{
		m_Rotation = rotation;
		ProcessVector();
	}

	void Camera::ProcessVector()
	{
		float yaw = glm::radians(m_Rotation.x);
		float pitch = glm::radians(m_Rotation.y);

		m_Forward.x = cos(yaw) * cos(pitch);
		m_Forward.y = sin(pitch);
		m_Forward.z = sin(yaw) * cos(pitch);
		m_Forward = glm::normalize(m_Forward);

		glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

		m_RightVector = glm::normalize(glm::cross(m_Forward, worldUp));
		m_UpVector = glm::normalize(glm::cross(m_RightVector, m_Forward));
	}

}
