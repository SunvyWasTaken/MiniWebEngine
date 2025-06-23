// Sunset inc.

#pragma once

#include "Mesh.h"

namespace Sunset
{
	class StaticMesh final : public Mesh
	{
	public:
		explicit StaticMesh(const StaticMeshData& data);

		virtual ~StaticMesh();

		NO_COPY_BUT_MOVE(StaticMesh)
	};
}
