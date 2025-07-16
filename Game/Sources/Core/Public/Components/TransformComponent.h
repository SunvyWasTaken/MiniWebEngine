// Sunset inc.

#pragma once

#include "BaseComponent.h"

namespace Sunset
{
	class TransformComponent : public BaseComponent
	{
		CALLBACKS_MULTI(FOnTransformChange, glm::vec3);

		friend class Scene;
	public:
		explicit TransformComponent(const glm::vec3& position = {0, 0, 0}, const glm::quat& rotation = glm::quat(1.f, 0.f, 0.f, 0.f));

		~TransformComponent();

		const glm::vec3& GetPosition() const { return m_Position; }

		void SetPosition(const glm::vec3& newPosition);

		void AddPosition(const glm::vec3& direction);

		const glm::quat GetRotation() const { return m_Rotation; }

		void AddPitch(float value);

		void AddYaw(float value);

		void AddRoll(float value);

		void SetRotation(const glm::vec3& rotation);

		void SetRotation(const glm::quat& rotation);

		const glm::vec3& GetSize() const { return m_Size; }

		void SetSize(const glm::vec3& newSize);

		const glm::mat4& GetModel() { if(bDirty) { ProcessModel(); } return m_Model; }

	private:

		void ProcessModel();

		void UpdatePhysic();

	public:

		FOnTransformChange OnTransformChange;

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Size;
		// Rotation in degree
		glm::quat m_Rotation;

		glm::mat4 m_Model;

		bool bDirty;
	};
}
