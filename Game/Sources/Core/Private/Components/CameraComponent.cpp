// Sunset inc.

#include "Components/CameraComponent.h"

#include "Prefab/Pawn.h"
#include "Camera.h"

namespace Sunset
{

	void CameraComponent::Update(float deltatime)
	{
		if (m_Camera && owner)
		{
			glm::vec3 pos = static_cast<Pawn*>(owner)->GetPosition() + m_Offset;
			m_Camera->SetPosition(pos);
		}
	}

	void CameraComponent::SetCamera(Camera* cam)
	{
		m_Camera = cam;
	}

}
