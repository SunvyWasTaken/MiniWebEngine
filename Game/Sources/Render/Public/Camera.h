// Sunset inc.

#pragma once

namespace Sunset
{
	class Camera final
	{
	public:
		Camera();

		void AddPosition(const glm::vec3& position);

		glm::mat4 GetView() const;

		glm::mat4 GetProjection() const;

		const glm::vec3& GetCameraPosition() const { return m_Position; }
		const glm::vec3& GetCameraForwardVector() const { return m_Forward; }
		const glm::vec3& GetCameraUpVector() const { return m_UpVector; }
		const glm::vec3 GetCameraRightVector() const { return glm::cross(m_Forward, m_UpVector); }

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Forward;
		glm::vec3 m_UpVector;
		glm::vec2 m_Resolution;

		float m_Fov;
		float m_RenderDistance;
	};
}
