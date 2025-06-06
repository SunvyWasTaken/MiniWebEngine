// Sunset inc.

#include "CubeTexture.h"

#include "glad/glad.h"

#define STD_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Sunset
{
	CubeTexture::~CubeTexture()
	{
		glDeleteTextures(GL_TEXTURE_CUBE_MAP, &m_Id);
	}

	bool CubeTexture::Load(const std::vector<std::string>& files)
	{
		glGenTextures(1, &m_Id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_Id);

		int width, height, nrChannels;
		for (unsigned int i = 0; i < files.size(); i++)
		{
			unsigned char* data = stbi_load(files[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
				);
				stbi_image_free(data);
			}
			else
			{
				ENGINE_LOG_ERROR("Cubemap tex failed to load at path: {}", files[i])
				stbi_image_free(data);
				return false;
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		return true;
	}

	void CubeTexture::Use() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_Id);
	}

}
