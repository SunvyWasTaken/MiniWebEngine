// Sunset inc.

#pragma once

#include "Mesh.h"
#include "Meshes/SkeletalBone.h"

namespace Sunset
{
	class AnimationClip;

	class SkeletalMesh final : public Mesh
	{
		friend class MeshLoader;
	public:
		SkeletalMesh();
		virtual ~SkeletalMesh();

		NO_COPY_BUT_MOVE(SkeletalMesh)

		void Update(float deltatime);

		void AddSubMesh(const SkeletalMeshData& data);

		// Todo : tmp
		void AddAnimation(std::shared_ptr<AnimationClip>& anim);

		Skeletal& GetSkel() {return m_Skeletal; }

	private:
		Skeletal m_Skeletal;
	};
}
