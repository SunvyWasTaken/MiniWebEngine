// Sunset inc.

#pragma once

namespace Sunset
{
	class Shader final
	{
	public:
		Shader(const char* vertShader, const char* fragShader);

		~Shader();

		void Use();

		void SetUniformMat4(const std::string& target, const glm::mat4& value);

		void SetUniformVec3(const std::string& target, const glm::vec3& value);

	private:
		uint32_t id;
	};
}
