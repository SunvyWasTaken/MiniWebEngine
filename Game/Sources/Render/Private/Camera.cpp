#include "Camera.h"
#include "Inputs.h"
#include "Render.h"

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

namespace Sunset
{
	Camera::Camera() 
		: CameraMovementSpeed(1.f)
		, pos(0.f, 0.f, -1.f)
		, res(1280.f, 720.f)
		, roll(0.f)
		, pitch(0.f)
		, yaw(0.f)
	{
	}

	Camera::~Camera()
	{
	}

	void Camera::Update(const double deltatime)
	{
		return;
	}

	glm::mat4 Camera::GetProjection() const
	{
		float halfHeight = 1;
		float aspect = float(res.x) / float(res.y);
		float halfWidth = halfHeight * aspect;

		return glm::ortho(
			-halfWidth, +halfWidth,
			-halfHeight, +halfHeight,
			-1.f, +100.f
		);
	}

	glm::mat4 Camera::GetView() const
	{
		glm::mat4 t = glm::translate(glm::mat4(1.f), pos);
		glm::mat4 r = glm::rotate(glm::mat4(1.f), glm::radians(roll), glm::vec3(0, 0, 1));
		return glm::inverse(r * t);
	}

	glm::mat4 Camera::GetProjectionView() const
	{
		return GetProjection() * GetView();
	}

	void Camera::AddPosition(const glm::vec2& _pos)
	{
		pos.x += _pos.x;
		pos.y += _pos.y;
	}

	glm::vec3 Camera::GetCurseurWorldPosition()
	{
		glm::vec2 screenPos = Inputs::GetMouseScreenPosition();
		glm::vec4 viewport(0, 0, (float)Render::GetWidth(), (float)Render::GetHeight());
		screenPos.y = res.y - screenPos.y;
		return glm::unProject(glm::vec3(screenPos.x, screenPos.y, 0.0f), GetView(), GetProjection(), viewport);
	}

}

