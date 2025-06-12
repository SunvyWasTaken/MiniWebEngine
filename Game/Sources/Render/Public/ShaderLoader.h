// Sunset inc.

#pragma once

namespace Sunset
{
	class Shader;

	struct ShaderLoader
	{
		static std::shared_ptr<Shader> Load(const std::string& name, const std::string& vertexFile = "", const std::string& fragmentFile = "");
	};
}
