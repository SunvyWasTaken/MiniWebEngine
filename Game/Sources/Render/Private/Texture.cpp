// Sunset inc.

#include "Texture.h"

#include "glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Sunset
{

	Texture::Texture()
	{

	}

	Texture::~Texture()
	{
		glDeleteTextures(GL_TEXTURE_2D, &m_Id);
	}

	bool Texture::LoadFromFile(const std::string& file)
	{
		glGenTextures(1, &m_Id);
		glBindTexture(GL_TEXTURE_2D, m_Id);

		int width, height, channels;
		stbi_set_flip_vertically_on_load(false);
		unsigned char* data = stbi_load(file.c_str(), &width, &height, &channels, 0);

		if (data)
		{
			GLenum format = (channels == 3) ? GL_RGB : GL_RGBA;
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
			return true;
		}

		stbi_image_free(data);
		ENGINE_LOG_FATAL("Erreur de chargement de l'image");
		return false;
	}

	void Texture::Use() const
	{
		glBindTexture(GL_TEXTURE_2D, m_Id);
	}

}
