// Sunset inc.

#pragma once

#include "Vertex.h"

namespace Sunset
{
	struct SubMesh final
	{
		SubMesh();
		~SubMesh();

		NO_COPY_BUT_MOVE(SubMesh)

		void Draw() const;

		uint32_t VAO = 0;
		uint32_t VBO = 0;
		uint32_t EBO = 0;

		size_t m_IndicesSize = 0;
	};

	class Mesh
	{
		friend class MeshLoader;
	public:
		Mesh();
		virtual ~Mesh();

		NO_COPY_BUT_MOVE(Mesh)

		void Draw() const;

	protected:
		uint32_t VAO = 0;
		uint32_t VBO = 0;
		uint32_t EBO = 0;

		size_t m_IndicesSize = 0;

		size_t m_ImportSize = 0;
	};
}
