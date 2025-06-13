// Sunset inc.

#include "Shaders.h"

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

#include <fstream>
#include <sstream>

namespace Sunset
{
	Shader::Shader(const char* vertShader, const char* fragShader)
		: id(0)
	{
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		vShaderFile.open(vertShader);
		fShaderFile.open(fragShader);
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.close();
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		unsigned int vertex, fragment;

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		id = glCreateProgram();

		glAttachShader(id, vertex);
		glAttachShader(id, fragment);
		glLinkProgram(id);
		int success;
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetProgramInfoLog(id, 512, NULL, infoLog);
			ENGINE_LOG_ERROR("ERROR::SHADER::PROGRAM::LINKING_FAILED\n{}", infoLog);
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	Shader::~Shader()
	{
		glDeleteProgram(id);
		ENGINE_LOG_TRACE("Shader {} Destroyed.", id)
	}

	void Shader::Use()
	{
		glUseProgram(id);
	}

	void Shader::SetUniformMat4(const std::string& target, const glm::mat4& value)
	{
		glUniformMatrix4fv(glGetUniformLocation(id, target.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::SetUniformVec3(const std::string& target, const glm::vec3& value)
	{
		glUniform3fv(glGetUniformLocation(id, target.c_str()), 1, glm::value_ptr(value));
	}

	void Shader::SetUniformInt1v(const std::string& target, int count, const int* value)
	{
		glUniform1iv(glGetUniformLocation(id, target.c_str()), count, value);
	}

}
