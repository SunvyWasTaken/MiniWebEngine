// Sunset inc.

#pragma once

namespace Sunset
{
	class TransformComponent
	{
	public:
		explicit TransformComponent(const glm::vec3& position = {0, 0, 0});

		~TransformComponent();

		const glm::vec3& GetPosition() const { return m_Position; }

		void SetPosition(const glm::vec3& newPosition);

		void AddPosition(const glm::vec3& direction);

		void AddPitch(float value);

		void AddYaw(float value);

		void AddRoll(float value);

		void SetRotation(const glm::vec3& rotation);

		const glm::vec3& GetSize() const { return m_Size; }

		void SetSize(const glm::vec3& newSize);

		const glm::mat4& GetModel() { if(bDirty) { ProcessModel(); } return m_Model; }

	private:

		void ProcessModel();

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Size;
		// Rotation in degree
		glm::vec3 m_Rotation;

		glm::mat4 m_Model;

		bool bDirty;
	};
}
