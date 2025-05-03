#pragma once

namespace Sunset
{
	class Camera;

	class Shader
	{
	public:
		Shader(const char* vertShader = "../../Game/Shaders/vShader.glsl", const char* fragShader = "../../Game/Shaders/fShader.glsl");
		~Shader();

		void operator()() const;

		void SetMatrice4f(const std::string_view& name, const glm::mat4& value) const;

		void SetVec2f(const std::string_view& name, const glm::vec2& value) const;

	private:
		uint32_t ID;
	};
}
