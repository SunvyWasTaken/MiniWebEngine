// Sunset inc.

#include "Animation/AnimationClip.h"
#include "Animation/AnimationTrack.h"

namespace Sunset
{

	AnimationClip::AnimationClip(const std::string& name, float duration, float ticksPerSecond)
		: m_Name(name)
		, m_Duration(duration)
		, m_TicksPerSecond(ticksPerSecond)
		, m_Tracks()
	{
	}

	const std::string& AnimationClip::GetName() const
	{
		return m_Name;
	}

	float AnimationClip::GetDuration() const
	{
		return m_Duration;
	}

	float AnimationClip::GetTicksPerSecond() const
	{
		return m_TicksPerSecond;
	}

	const std::vector<std::shared_ptr<Sunset::AnimationTrack>>& AnimationClip::GetTracks() const
	{
		return m_Tracks;
	}

	void AnimationClip::AddTrack(const std::shared_ptr<AnimationTrack>& track)
	{
		m_Tracks.emplace_back(track);
	}

}
