#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

#include <iostream>

namespace Sunset
{
	Texture::Texture(const std::string& path)
		: m_SubUv(glm::vec2{1, 1})
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		stbi_set_flip_vertically_on_load(true);

		int width, height, nrChannels;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &texture);
	}

	void Texture::operator()()
	{
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	void Texture::SetSubUv(const glm::vec2& uv)
	{
		m_SubUv = uv;
	}

	glm::vec2 Texture::GetSubUv() const
	{
		return {1.f / m_SubUv.x, 1.f / m_SubUv.y} ;
	}

}
