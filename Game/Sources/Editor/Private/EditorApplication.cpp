// Sunset inc.

#include "EditorApplication.h"
#include "Inputs.h"
#include "Camera.h"

namespace Sunset
{
	void EditorEngine::LogicLoop(const float deltatime)
	{
		if (Inputs::IsKey(87))
		{
			glm::vec3 dir = GetCam().GetCameraUpVector() * deltatime;
			GetCam().AddPosition(dir);
		}
		if (Inputs::IsKey(83))
		{
			glm::vec3 dir = GetCam().GetCameraUpVector() * deltatime;
			GetCam().AddPosition(-dir);
		}
		if (Inputs::IsKey(68))
		{
			glm::vec3 dir = GetCam().GetCameraRightVector() * deltatime;
			GetCam().AddPosition(-dir);
		}
		if (Inputs::IsKey(65))
		{
			glm::vec3 dir = GetCam().GetCameraRightVector() * deltatime;
			GetCam().AddPosition(dir);
		}

		if (Inputs::IsKey(71))
		{
			Engine::LogicLoop(deltatime);
		}
	}
}
