#pragma once

#include "Object.h"
#include "Shader.h"
#include "Texture.h"

#include <glm/glm.hpp>

#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace Sunset
{
	class Camera;

	struct Vertice
	{
		glm::vec2 pos;
		glm::vec2 TextureCoord;
		glm::vec4 Color;
	};

	namespace Object
	{
		struct Line {};
		struct Triangle {};
		using Type = std::variant<Line, Triangle>;
	}
	
	class RenderObject
	{
	public:
		RenderObject();
		virtual ~RenderObject();
		void PostInit();
		virtual void PostRender();
		void operator()(const TransformComponent& transform, const std::shared_ptr<Camera>& cam);
	protected:
		std::vector<Vertice> vertices;
		std::vector<uint32_t> indexBuffer;
		Object::Type type;

		uint32_t VAO;
		uint32_t VBO;
		uint32_t EBO;

		Shader shader;
		Texture texture;

		int i = 1;
		int j = i;
		double time = 0;
	};

	class Line : public RenderObject
	{
	public:
		Line();
	};

	class Triangle : public RenderObject
	{
	public:
		Triangle();
	};

	class Square : public RenderObject
	{
	public:
		Square();
	};

	class Sprite : public RenderObject
	{
	public:
		virtual void PostRender() override;
	};

}
