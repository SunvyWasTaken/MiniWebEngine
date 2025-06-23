// Sunset inc.

#pragma once

#include "Meshes/Bone.h"

namespace Sunset
{
	using Bones = std::vector<Bone>;
	
	struct Skeletal final
	{
		Skeletal();

		~Skeletal();

		NO_COPY_BUT_MOVE(Skeletal)

		void Update(float deltatime);

		void UpdateGPU() const;

		uint32_t operator()() const { return m_Ubo; }

	public:

		Bones bones;

		std::unordered_map<std::string, int> boneNameToIndex;

		std::vector<glm::mat4> finalBoneMatrices;

	private:
		uint32_t m_Ubo;
	};
}
