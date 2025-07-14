// Sunset inc.

#pragma once

#include "BaseComponent.h"

namespace Sunset
{
	class Camera;

	class CameraComponent : public BaseComponent
	{
	public:
		void Update(float deltatime);

		void SetCamera(Camera* cam);
	private:
		Camera* m_Camera;
		glm::vec3 m_Offset;
	};
}
