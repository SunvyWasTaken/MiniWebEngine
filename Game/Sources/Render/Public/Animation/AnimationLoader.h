// Sunset inc.

#pragma once

namespace Sunset
{
	class AnimationClip;
	struct Bone;

	struct AnimLoader
	{
		static std::shared_ptr<AnimationClip> LoadAnimation(const std::string& path, const std::vector<Bone>& bone);
	};
}
