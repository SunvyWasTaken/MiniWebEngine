// Sunset inc.

#pragma once

namespace Sunset
{
	enum class Projection
	{
		Perspective,
		Orthographique
	};

	class Camera final
	{
	public:
		Camera();

		void AddPosition(const glm::vec3& position);

		void SetPosition(const glm::vec3& newPosition);

		glm::mat4 GetView() const;

		glm::mat4 GetProjection() const;

		glm::mat4 GetRelativeView() const;

		const glm::vec3& GetRotation() const { return m_Rotation; }

		void SetRotation(const glm::vec3& rotation);

		const glm::vec3& GetCameraPosition() const { return m_Position; }
		const glm::vec3& GetCameraForwardVector() const { return m_Forward; }
		const glm::vec3& GetCameraUpVector() const { return m_UpVector; }
		const glm::vec3& GetCameraRightVector() const { return m_RightVector; }

	private:

		void ProcessVector();

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Forward;
		glm::vec3 m_RightVector;
		glm::vec3 m_UpVector;
		glm::vec2 m_Resolution;
		glm::vec3 m_Rotation;

		float m_Fov;
		float m_RenderDistance;

		Projection m_Projection;
	};
}
