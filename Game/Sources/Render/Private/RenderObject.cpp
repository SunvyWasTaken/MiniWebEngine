#include "RenderObject.h"
#include "Camera.h"
#include "Components.h"

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

namespace
{
	struct RenderData
	{
		RenderData()
			: bIsLoad(false)
			, bUseEBO(false)
			, VAO(0)
			, VBO(0)
			, EBO(0)
			, verticeSize(0)
			, indexSize(0)
		{ }

		RenderData(const std::vector<Sunset::Vertice>& vertices, const std::vector<uint32_t>& indexBuffer)
			: bIsLoad(true)
			, bUseEBO(!indexBuffer.empty())
			, VAO(0)
			, VBO(0)
			, EBO(0)
			, verticeSize(vertices.size())
			, indexSize(indexBuffer.size())
		{
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);

			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Sunset::Vertice), vertices.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Sunset::Vertice), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Sunset::Vertice), (void*)(sizeof(glm::vec2)));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Sunset::Vertice), (void*)(2 * sizeof(glm::vec2)));
			glEnableVertexAttribArray(2);

			if (bUseEBO)
			{
				glGenBuffers(1, &EBO);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.size() * sizeof(uint32_t), indexBuffer.data(), GL_STATIC_DRAW);
			}
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		~RenderData()
		{
			glDeleteBuffers(1, &VBO);
			if (bUseEBO)
			{
				glDeleteBuffers(1, &EBO);
			}
			glDeleteVertexArrays(1, &VAO);
		}

		operator bool()
		{
			return bIsLoad;
		}

		void Use()
		{
			glBindVertexArray(VAO);
			if (bUseEBO)
				glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);
			else
				glDrawArrays(GL_TRIANGLES, 0, verticeSize);
		}

		bool bIsLoad, bUseEBO;
		uint32_t VAO, VBO, EBO;
		size_t verticeSize, indexSize;
	};

	RenderData data;
}

std::shared_ptr<RenderData> ObjectData = nullptr;

namespace Sunset
{
	RenderObject::RenderObject()
#ifdef __EMSCRIPTEN__
		: texture("Ressources/SpriteSheet.png")
#else
		: texture("../../Ressources/SpriteSheet.png")
#endif
	{
		texture.SetSubUv({4, 4});
	}

	RenderObject::~RenderObject()
	{}

	void RenderObject::PostInit(const std::vector<Vertice>& vertices, const std::vector<uint32_t>& index)
	{
		if (!ObjectData)
		{
			ObjectData = std::make_shared<RenderData>(vertices, index);
		}
	}

	void RenderObject::PostRender() const
	{
	}

	void RenderObject::operator()(const TransformComponent& transform, const std::shared_ptr<Camera>& cam) const
	{
		shader();

		glm::mat4 model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(transform.location.x, transform.location.y, 1.f));
		model = glm::scale(model, glm::vec3(transform.size.x, transform.size.y, 1.f));
		model = glm::rotate(model, glm::radians(transform.rot), {0, 0, 1});
		shader.SetMatrice4f("model", model);

		shader.SetMatrice4f("viewProj", cam->GetProjectionView());
		shader.SetVec2f("SubUv", texture.GetSubUv());

		texture();

		PostRender();

		ObjectData->Use();
	}

	Square::Square(const glm::vec2& coord)
		: RenderObject()
		, SpriteCoord(coord)
	{
		const std::vector<Vertice> vertices =
		{ {{ 0.5f,  0.5f}, {1.f, 1.f}, {1, 1, 1, 1}}
		, {{ 0.5f, -0.5f}, {1.f, 0.f}, {1, 0, 0, 1}}
		, {{-0.5f, -0.5f}, {0.f, 0.f}, {0, 1, 0, 1}}
		, {{-0.5f,  0.5f}, {0.f, 1.f}, {0, 0, 1, 1}}
		};
		const std::vector<uint32_t> indexBuffer = {0, 1, 3, 1, 2, 3};

		PostInit(vertices, indexBuffer);
	}

	void Square::PostRender() const
	{
		shader.SetVec2f("index", SpriteCoord);
	}
}
