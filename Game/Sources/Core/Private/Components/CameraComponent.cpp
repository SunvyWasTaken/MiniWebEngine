// Sunset inc.

#include "Components/CameraComponent.h"

#include "Prefab/Pawn.h"
#include "Camera.h"

namespace Sunset
{
	CameraComponent::CameraComponent()
		: m_Camera(std::make_unique<Camera>())
	{
		
	}

	CameraComponent::~CameraComponent()
	{
	}

	void CameraComponent::SetOffset(const glm::vec3& offset)
	{
		m_Offset = offset;
	}

	void CameraComponent::SetCamera(Camera* cam)
	{
		m_Camera.reset();
		m_Camera = std::unique_ptr<Camera>(cam);
	}

	glm::vec3 CameraComponent::GetForwardVector() const
	{
		return m_Camera->GetCameraForwardVector();
	}

	void CameraComponent::AddRotation(const glm::vec3& rotation)
	{
		const glm::vec3 rot = m_Camera->GetRotation() + rotation;
		m_Camera->SetRotation(rot);
	}

	void CameraComponent::UpdatePosition(const glm::vec3& pos)
	{
		glm::vec3 newPos = pos + m_Offset;
		m_Camera->SetPosition(newPos);
	}

}
