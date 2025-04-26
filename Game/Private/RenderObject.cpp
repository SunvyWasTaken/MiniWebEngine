#include "RenderObject.h"
#include "Camera.h"

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <memory>

namespace Sunset
{
	template <class ...Ts>
	struct overload : Ts...
	{
		using Ts::operator()...;
	};

	template <class ...Ts>
	overload(Ts...) -> overload<Ts...>;

	RenderObject::RenderObject()
		: VAO(0)
		, VBO(0)
		, EBO(0)
#ifdef __EMSCRIPTEN__
		, texture("Ressources/SpriteSheet.png")
#else
		, texture("../../Ressources/SpriteSheet.png")
#endif
	{
		texture.SetSubUv({4, 4});
	}

	RenderObject::~RenderObject()
	{
		glDeleteBuffers(1, &VBO);
		if (!indexBuffer.empty())
		{
			glDeleteBuffers(1, &EBO);
		}
		glDeleteVertexArrays(1, &VAO);
	}

	void RenderObject::PostInit()
	{
		if (vertices.empty())
			return;

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertice), vertices.data(), GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertice), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertice), (void*)(sizeof(glm::vec2)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertice), (void*)(2 * sizeof(glm::vec2)));
		glEnableVertexAttribArray(2);

		if (!indexBuffer.empty())
		{
			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.size() * sizeof(uint32_t), indexBuffer.data(), GL_DYNAMIC_DRAW);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void RenderObject::PostRender()
	{
	}

	void RenderObject::operator()(const TransformComponent& transform, const std::shared_ptr<Camera>& cam)
	{
		shader();

		glm::mat4 model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(transform.location.x, transform.location.y, 1.f));
		model = glm::scale(model, glm::vec3(transform.size.x, transform.size.y, 1.f));
		shader.SetMatrice4f("model", model);

		shader.SetMatrice4f("viewProj", cam->GetProjectionView());
		shader.SetVec2f("SubUv", texture.GetSubUv());

		texture();

		PostRender();

		glBindVertexArray(VAO);
		if (!indexBuffer.empty())
		{
			glDrawElements(GL_TRIANGLES, indexBuffer.size(), GL_UNSIGNED_INT, 0);
		}
		else
		{
			std::visit(overload{
			[&](Object::Line tmp)
			{
				glDrawArrays(GL_LINES, 0, vertices.size());
			},
			[&](Object::Triangle tmp)
			{
				glDrawArrays(GL_TRIANGLES, 0, vertices.size());
			}
			}, type);
		}
	}

	Triangle::Triangle()
		: RenderObject()
	{
		vertices =
		{ {{-0.5f, -0.5f}, {0.5f, 1.0f}, {1, 1, 1, 1}}
		, {{ 0.5f, -0.5f}, {0.0f, 0.0f}, {1, 1, 1, 1}}
		, {{ 0.0f,  0.5f}, {1.0f, 0.0f}, {1, 1, 1, 1}}
		};
		indexBuffer = {0, 1, 2};

		PostInit();
	}

	Square::Square(const glm::vec2& coord)
		: RenderObject()
		, SpriteCoord(coord)
	{
		vertices =
		{ {{ 0.5f,  0.5f}, {1.f, 1.f}, {1, 1, 1, 1}}
		, {{ 0.5f, -0.5f}, {1.f, 0.f}, {1, 0, 0, 1}}
		, {{-0.5f, -0.5f}, {0.f, 0.f}, {0, 1, 0, 1}}
		, {{-0.5f,  0.5f}, {0.f, 1.f}, {0, 0, 1, 1}}
		};
		indexBuffer = {0, 1, 3, 1, 2, 3};

		PostInit();
	}

	void Square::PostRender()
	{
		shader.SetVec2f("index", SpriteCoord);
	}

	Line::Line()
	{
		PostInit();
	}

	void Sprite::PostRender()
	{
		
	}

}
