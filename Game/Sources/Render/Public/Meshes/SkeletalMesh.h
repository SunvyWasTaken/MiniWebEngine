// Sunset inc.

#pragma once

#include "Mesh.h"
#include "Meshes/SkeletalBone.h"

namespace Sunset
{
	class SkeletalMesh final : public Mesh
	{
		friend class MeshLoader;
	public:
		SkeletalMesh();
		virtual ~SkeletalMesh();

		NO_COPY_BUT_MOVE(SkeletalMesh)

		void Update(float deltatime);

		void AddSubMesh(const SkeletalMeshData& data);

	private:
		Skeletal m_Skeletal;
	};
}
