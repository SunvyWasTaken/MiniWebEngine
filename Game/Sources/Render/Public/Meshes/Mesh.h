// Sunset inc.

#pragma once

#include "Vertex.h"

namespace Sunset
{
	struct SubMesh final
	{
		SubMesh(uint32_t vao, uint32_t vbo, uint32_t ebo, size_t size);
		~SubMesh();

		NO_COPY_BUT_MOVE(SubMesh)

		void Draw() const;

	private:

		uint32_t VAO, VBO, EBO;

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

		float GetSize() const { return m_ImportSize; }

	protected:

		std::vector<SubMesh> m_SubMeshes;

		float m_ImportSize;
	};
}
