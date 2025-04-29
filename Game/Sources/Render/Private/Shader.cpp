#include "Shader.h"
#include "Camera.h"

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

namespace Sunset
{
	Shader::Shader(const char* vertexPath /*= "../../Game/Shaders/vShader.vert"*/, const char* fragmentPath /*= "../../Game/Shaders/fShader.frag"*/)
	{
#ifdef __EMSCRIPTEN__
		vertexPath = "Shaders/vShader_Web.glsl";
		fragmentPath= "Shaders/fShader_Web.glsl";
#endif
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.close();
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		uint32_t vertex, fragment;

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		ID = glCreateProgram();

		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		int success;
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	Shader::~Shader()
	{
		glDeleteProgram(ID);
	}

	void Shader::operator()()
	{
		glUseProgram(ID);
	}

	void Shader::SetMatrice4f(const std::string_view& name, const glm::mat4& value)
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.data()), 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::SetVec2f(const std::string_view& name, const glm::vec2& value)
	{
		glUniform2fv(glGetUniformLocation(ID, name.data()), 1, glm::value_ptr(value));
	}

}