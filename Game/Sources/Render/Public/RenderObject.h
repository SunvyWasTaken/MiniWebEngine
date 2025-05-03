#pragma once

#include "Components.h"
#include "Shader.h"
#include "Texture.h"

namespace Sunset
{
	class Camera;

	struct Vertice
	{
		glm::vec2 pos;
		glm::vec2 TextureCoord;
		glm::vec4 Color;
	};
	
	class RenderObject
	{
	public:
		RenderObject();

		virtual ~RenderObject();

		void PostInit(const std::vector<Vertice>& vertices, const std::vector<uint32_t>& index = std::vector<uint32_t>());

		virtual void PostRender() const;

		void operator()(const Sunset::TransformComponent& transform, const std::shared_ptr<Camera>& cam) const;

	protected:

		Shader shader;
		Texture texture;

		int i = 1;
		int j = i;
		double time = 0;
	};

	class Square : public RenderObject
	{
	public:
		explicit Square(const glm::vec2& coord);
		virtual void PostRender() const override;
		glm::vec2 SpriteCoord;
	};

}
