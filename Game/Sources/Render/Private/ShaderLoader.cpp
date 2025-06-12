// Sunset inc.

#include "ShaderLoader.h"
#include "Shaders.h"

namespace
{
	std::unordered_map<std::string, std::weak_ptr<Sunset::Shader>> m_ShadersList;
}

namespace Sunset
{
	std::shared_ptr<Shader> ShaderLoader::Load(const std::string& name, const std::string& vertexFile, const std::string& fragmentFile)
	{
		auto it = m_ShadersList.find(name);
		if (it != m_ShadersList.end())
		{
			if (std::shared_ptr<Shader> currShader = it->second.lock())
			{
				return currShader;
			}
			else
			{
				m_ShadersList.erase(name);
			}
		}
		std::shared_ptr<Shader> currShader = std::make_shared<Shader>(vertexFile.c_str(), fragmentFile.c_str());
		m_ShadersList.insert_or_assign(name, currShader);
		return currShader;
	}
}
