#include "TraceObject.h"
#include "Shader.h"

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

#define __VAO__ traceObjectData.VAO
#define __VBO__ traceObjectData.VBO
#define __SHADER__ traceObjectData.shader

namespace
{
	struct DrawData
	{
		uint32_t VAO = 0;
		uint32_t VBO = 0;
		std::unique_ptr<Sunset::Shader> shader = nullptr;
		bool isLoad = false;
	};

	DrawData traceObjectData;
	constexpr int MaxSize = 1000;
}

namespace Sunset
{
	TraceObject::TraceObject()
	{
		if (traceObjectData.isLoad)
			return;

		points.reserve(MaxSize);

		traceObjectData.isLoad = true;
		glGenVertexArrays(1, &__VAO__);
		glGenBuffers(1, &__VBO__);

		glBindVertexArray(__VAO__);
		glBindBuffer(GL_ARRAY_BUFFER, __VBO__);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * MaxSize, nullptr, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
#ifdef __EMSCRIPTEN__
		__SHADER__ = std::make_unique<Sunset::Shader>("Shaders/vShaderTrace_Web.glsl", "Shaders/fShaderTrace_Web.glsl");
#else
		__SHADER__ = std::make_unique<Sunset::Shader>("../../Game/Shaders/vShaderTrace.glsl", "../../Game/Shaders/fShaderTrace.glsl");
#endif
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	TraceObject::~TraceObject()
	{
		glDeleteBuffers(1, &__VBO__);
		glDeleteVertexArrays(1, &__VAO__);
	}

	void TraceObject::AddPoints(const glm::vec2& point)
	{
		if (points.size() >= MaxSize)
			return;

		points.push_back(point);

		glBindBuffer(GL_ARRAY_BUFFER, __VBO__);
		glBufferSubData(GL_ARRAY_BUFFER, 0, points.size() * sizeof(glm::vec2), points.data());
	}

	void TraceObject::Draw()
	{
		if (points.empty())
			return;

		__SHADER__->Use();
		glBindVertexArray(__VAO__);
		glDrawArrays(GL_LINE_STRIP, 0, points.size());

		glBindVertexArray(0);
	}

	void TraceObject::Clear()
	{
		points.clear();
	}

	TraceObject::operator PointsList()
	{
		return points;
	}

	TraceObject::operator bool()
	{
		return !points.empty();
	}

}
