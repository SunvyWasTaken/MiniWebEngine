// Sunset inc.

#pragma once

#include "Mesh.h"
#include "Meshes/SkeletalBone.h"

namespace Sunset
{
	class SkeletalMesh final : public Mesh
	{
	public:
		explicit SkeletalMesh(const SkeletalMeshData& data);
		virtual ~SkeletalMesh();

		NO_COPY_BUT_MOVE(SkeletalMesh)

		void Update(float deltatime);

	private:
		Skeletal m_Skeletal;
	};
}
