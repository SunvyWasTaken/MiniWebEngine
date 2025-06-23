// Sunset inc.

#include "Shaders.h"

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

#include <fstream>
#include <sstream>
#include <filesystem>

namespace
{
	std::string PreprocessShader(const std::string& path, const std::string& parentDir = "")
	{
		std::ifstream file(path);
		if (!file.is_open()) {
			ENGINE_LOG_INFO("Erreur : impossible d'ouvrir le fichier shader : {}", path);
			return "";
		}

		std::stringstream processedShader;
		std::string line;
		std::string baseDir = parentDir.empty() ? std::filesystem::path(path).parent_path().string() : parentDir;

		while (std::getline(file, line)) {
			if (line.find("#include") == 0) {
				size_t firstQuote = line.find('\"');
				size_t lastQuote = line.rfind('\"');
				if (firstQuote != std::string::npos && lastQuote != std::string::npos && lastQuote > firstQuote) {
					std::string includeFile = line.substr(firstQuote + 1, lastQuote - firstQuote - 1);
					std::string includePath = baseDir + "/" + includeFile;

					std::string includedCode = PreprocessShader(includePath, std::filesystem::path(includePath).parent_path().string());
					processedShader << "// Begin include: " << includeFile << "\n";
					processedShader << includedCode << "\n";
					processedShader << "// End include: " << includeFile << "\n";
				}
				else {
					ENGINE_LOG_INFO("Erreur de syntaxe dans le #include : {}", line);
				}
			}
			else {
				processedShader << line << "\n";
			}
		}

		return processedShader.str();
	}
}

namespace Sunset
{
	Shader::Shader(const char* vertShader, const char* fragShader)
		: id(0)
	{
		std::string vertexCode = PreprocessShader(vertShader);
		std::string fragmentCode = PreprocessShader(fragShader);

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

		ENGINE_LOG_TRACE("Shader Create : {}, {}", id, vertShader)

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

	void Shader::Use() const
	{
		glUseProgram(id);
	}

	void Shader::SetUniformMat4(const std::string& target, const glm::mat4& value) const
	{
		glUniformMatrix4fv(glGetUniformLocation(id, target.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::SetUniformVec3(const std::string& target, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(id, target.c_str()), 1, glm::value_ptr(value));
	}

	void Shader::SetUniformInt1v(const std::string& target, int count, const int* value) const
	{
		glUniform1iv(glGetUniformLocation(id, target.c_str()), count, value);
	}

}
