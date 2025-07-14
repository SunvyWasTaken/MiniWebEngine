// Sunset inc.

#include "CameraManager.h"
#include "Camera.h"

namespace
{
	Sunset::Camera* m_ActiveCamera = nullptr;
	Sunset::Camera backup_cam;
}

namespace Sunset
{

	void CameraManager::SetActiveCamera(Camera* cam)
	{
		m_ActiveCamera = cam;
	}

	Camera* CameraManager::GetActiveCamera()
	{
		if (m_ActiveCamera)
			return m_ActiveCamera;
		else
			return &backup_cam;
	}

}
