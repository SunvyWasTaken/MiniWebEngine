// Sunset inc.

#include "Texts/FontLoader.h"
#include "TextCharacter.h"
#include "Shaders.h"

#include "ft2build.h"
#include FT_FREETYPE_H

#include "glad/glad.h"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

namespace
{
	struct FreeType final
	{
		FT_Library ft;

		FreeType()
		{
			if (FT_Init_FreeType(&ft))
			{
				ENGINE_LOG_FATAL("ERROR::FREETYPE: Could not init FreeType Library")
				return;
			}
		}

		~FreeType()
		{
			FT_Done_FreeType(ft);
		}

		operator FT_Library() const
		{
			return ft;
		}
	};

	struct FreeFont final
	{
		FT_Face face;

		FreeFont(const FreeType& ft, const std::string& path)
		{
			if (FT_New_Face(ft, path.c_str(), 0, &face))
			{
				ENGINE_LOG_FATAL("ERROR::FREETYPE: Failed to load font");
				return;
			}
		}

		~FreeFont()
		{
			FT_Done_Face(face);
		}

		operator FT_Face&()
		{
			return face;
		}

		FT_Face& operator->()
		{
			return face;
		}
	};

	struct CharacterSquare final
	{
		uint32_t VAO, VBO;
		CharacterSquare()
		{
			glCreateVertexArrays(1, &VAO);
			glCreateBuffers(1, &VBO);

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			ENGINE_LOG_TRACE("Square mesh for character is create")
		}

		~CharacterSquare()
		{
			glDeleteBuffers(1, &VBO);
			glDeleteVertexArrays(1, &VAO);
			ENGINE_LOG_TRACE("Square mesh for character is Delete")
		}

		void Use() const
		{
			glBindVertexArray(VAO);
		}
	};

	Sunset::TextCharacters Characters;
	std::unique_ptr<CharacterSquare> meshCharacter = nullptr;
}

namespace Sunset
{
	void FontLoader::LoadFont(const std::string& path)
	{
		FreeType ft;
		FreeFont ff{ft, path};
		if (!ff.face) {
			ENGINE_LOG_FATAL("Font face is null");
			return;
		}
		if (FT_Set_Pixel_Sizes(ff, 0, 72)) {
			ENGINE_LOG_FATAL("Failed to set pixel size");
			return;
		}

		for (unsigned char c = 0; c < 128; ++c)
		{
			// load character glyph 
			if (FT_Load_Char(ff, c, FT_LOAD_RENDER))
			{
				ENGINE_LOG_ERROR("ERROR::FREETYTPE: Failed to load Glyph for char '{}'", static_cast<char>(c));
				continue;
			}
			// ENGINE_LOG_TRACE("Loaded Glyph '{}' size {}x{}", c, ff->glyph->bitmap.width, ff->glyph->bitmap.rows);

			// generate texture
			uint32_t texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				ff->glyph->bitmap.width,
				ff->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				ff->glyph->bitmap.buffer
			);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// now store character for later use
			Sunset::TextCharacter character = {
				texture,
				glm::ivec2(ff->glyph->bitmap.width, ff->glyph->bitmap.rows),
				glm::ivec2(ff->glyph->bitmap_left, ff->glyph->bitmap_top),
				static_cast<uint32_t>(ff->glyph->advance.x)
			};
			Characters.emplace(static_cast<char>(c), character);
		}
	}

	void FontLoader::RenderText(const std::shared_ptr<Shader>& shader, const std::string& text, float x, float y, float scale, const glm::vec3& color)
	{
		if(!meshCharacter)
		{
			meshCharacter = std::make_unique<CharacterSquare>();
		}

		glm::mat4 projection = glm::ortho(0.0f, 1280.f, 0.0f, 720.0f);

		shader->Use();
		shader->SetUniformVec3("textColor", color);
		shader->SetUniformMat4("projection", projection);
		glActiveTexture(GL_TEXTURE0);
		meshCharacter->Use();

		// iterate through all characters
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); ++c)
		{
			TextCharacter ch = Characters[*c];

			float xpos = x + ch.bearing.x * scale;
			float ypos = y - (ch.size.y - ch.bearing.y) * scale;

			float w = ch.size.x * scale;
			float h = ch.size.y * scale;
			// update VBO for each character
			float vertices[6][4] = {
				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos,     ypos,       0.0f, 1.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },

				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },
				{ xpos + w, ypos + h,   1.0f, 0.0f }
			};
			// render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.textureId);
			// update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, meshCharacter->VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 6 * 4, vertices);
			GLenum err;
			while ((err = glGetError()) != GL_NO_ERROR) {
				ENGINE_LOG_ERROR("OpenGL Error during buffer update: {}", err);
			}
			// render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
