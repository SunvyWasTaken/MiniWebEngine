// Sunset inc.

#include "CameraManager.h"
#include "Camera.h"

namespace
{
	Sunset::Camera* m_ActiveCamera = nullptr;
}

namespace Sunset
{

	void CameraManager::SetActiveCamera(Camera* cam)
	{
		m_ActiveCamera = cam;
	}

	Camera* CameraManager::GetActiveCamera()
	{
		return m_ActiveCamera;
	}

}
