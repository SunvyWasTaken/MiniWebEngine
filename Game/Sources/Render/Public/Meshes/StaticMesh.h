// Sunset inc.

#pragma once

#include "Mesh.h"

namespace Sunset
{
	class StaticMesh final : public Mesh
	{
	public:
		explicit StaticMesh(const StaticVertices& data);

		NO_COPY_BUT_MOVE(StaticMesh);

		virtual ~StaticMesh();

		void Draw() const;
	};
}
