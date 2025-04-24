#pragma once

#include <glm/glm.hpp>

#include <memory>
#include <string>

namespace Sunset
{
	class Camera;

	class Shader
	{
	public:
		Shader(const char* vertShader = "../../Game/Shaders/vShader.glsl", const char* fragShader = "../../Game/Shaders/fShader.glsl");
		~Shader();

		void operator()();

		void SetMatrice4f(const std::string_view& name, const glm::mat4& value);

		void SetVec2f(const std::string_view& name, const glm::vec2& value);

	private:
		uint32_t ID;
	};
}
