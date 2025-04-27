#pragma once

namespace Sunset
{
	class Camera final
	{
	public:

		Camera();
		~Camera();

		void Update(const double deltatime);

		void AddPosition(const glm::vec2& _pos);

		glm::vec3 GetCurseurWorldPosition();

		glm::mat4 GetProjectionView() const;

	protected:

		glm::mat4 GetProjection() const;

		glm::mat4 GetView() const;

	public:

		float CameraMovementSpeed;

	private:

		glm::vec3 pos;
		glm::vec2 res;

		float roll;
		float pitch;
		float yaw;
	};
}
