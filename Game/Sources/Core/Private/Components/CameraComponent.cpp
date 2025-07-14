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

	void CameraComponent::Update(float deltatime)
	{
		if (m_Camera && owner)
		{
			glm::vec3 pos = static_cast<Pawn*>(owner)->GetPosition() + m_Offset;
			m_Camera->SetPosition(pos);
		}
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

}
