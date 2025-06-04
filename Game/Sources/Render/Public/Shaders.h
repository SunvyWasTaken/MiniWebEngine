// Sunset inc.

#pragma once

namespace Sunset
{
	class Shader
	{
	public:
		Shader(const char* vertShader, const char* fragShader);

		~Shader();

		void Use();

		void SetUniformMat4(const std::string& target, const glm::mat4& value);

	private:
		uint32_t id;
	};
}
