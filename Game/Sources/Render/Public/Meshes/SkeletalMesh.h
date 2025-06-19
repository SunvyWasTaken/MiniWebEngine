// Sunset inc.

#pragma once

#include "Mesh.h"

namespace Sunset
{
	class SkeletalMesh final : public Mesh
	{
	public:
		explicit SkeletalMesh(const SkeletalVertices& data);
		virtual ~SkeletalMesh();

		void Draw() const;
		void Update(float deltatime);
	};
}
