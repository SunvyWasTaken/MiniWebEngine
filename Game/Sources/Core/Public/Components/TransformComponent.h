// Sunset inc.

#pragma once

namespace Sunset
{
	class TransformComponent
	{
	public:
		explicit TransformComponent(const glm::vec3& position = {0, 0, 0});

		~TransformComponent();

		glm::vec3 GetPosition() const;

		void SetPosition(const glm::vec3& newPosition);

		void AddPosition(const glm::vec3& direction);

		glm::mat4 GetModel() const { return m_Model; }

	private:

		void ProcessModel();

	private:
		glm::vec3 m_Position;

		glm::mat4 m_Model;
	};
}
