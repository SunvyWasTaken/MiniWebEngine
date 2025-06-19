// Sunset inc.

#pragma once

#include "Vertex.h"

namespace Sunset
{
	class Mesh
	{
	public:
		Mesh();
		virtual ~Mesh();

		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;

		Mesh(Mesh&& other) noexcept;
		Mesh& operator=(Mesh&& other) noexcept;

	protected:
		uint32_t VAO = 0;
		uint32_t VBO = 0;
		uint32_t EBO = 0;

		size_t m_IndicesSize = 0;
	};
}
