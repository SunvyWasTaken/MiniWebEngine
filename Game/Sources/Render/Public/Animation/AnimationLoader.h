// Sunset inc.

#pragma once

namespace Sunset
{
	class AnimationClip;

	struct AnimLoader
	{
		static std::shared_ptr<AnimationClip> LoadAnimation(const std::string& path);
	};
}
