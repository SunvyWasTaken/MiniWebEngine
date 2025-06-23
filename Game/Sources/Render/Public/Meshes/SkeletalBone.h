// Sunset inc.

#pragma once

#include "Meshes/Bone.h"

namespace Sunset
{
	using Bones = std::vector<Bone>;
	
	struct Skeletal
	{
		Bones bones;
		std::unordered_map<std::string, int> boneNameToIndex;
	};
}
