// Sunset inc.

#pragma once

namespace Sunset
{
	class Shader final
	{
	public:
		Shader(const char* vertShader, const char* fragShader);

		~Shader();

		void Use() const;

		void SetUniformMat4(const std::string& target, const glm::mat4& value) const;

		void SetUniformVec3(const std::string& target, const glm::vec3& value) const;

		void SetUniformInt1v(const std::string& target, int count, const int* value) const;

	private:
		uint32_t id;
	};
}
