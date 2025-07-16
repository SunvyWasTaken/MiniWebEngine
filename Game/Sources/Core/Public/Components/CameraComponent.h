// Sunset inc.

#pragma once

#include "BaseComponent.h"

namespace Sunset
{
	class Camera;

	class CameraComponent : public BaseComponent
	{
	public:
		CameraComponent();

		virtual ~CameraComponent();

		void SetOffset(const glm::vec3& offset);

		void SetCamera(Camera* cam);

		Camera* GetCamera() const { return m_Camera.get(); }

		glm::vec3 GetForwardVector() const;

		void AddRotation(const glm::vec3& rotation);

		void UpdatePosition(const glm::vec3& pos);

	private:
		std::unique_ptr<Camera> m_Camera;
		glm::vec3 m_Offset;
	};
}
