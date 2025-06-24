// Sunset inc.

#pragma once

#include "Mesh.h"

namespace Sunset
{
	class StaticMesh final : public Mesh
	{
	public:
		StaticMesh();

		virtual ~StaticMesh();

		NO_COPY_BUT_MOVE(StaticMesh)

		void AddSubMesh(const StaticMeshData& data);
	};
}
