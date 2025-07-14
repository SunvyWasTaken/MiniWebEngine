// Sunset inc.

#pragma once

namespace Sunset
{
	class Camera;

	struct CameraManager
	{
		static void SetActiveCamera(Camera* cam);
		static Camera* GetActiveCamera();
	};
}
