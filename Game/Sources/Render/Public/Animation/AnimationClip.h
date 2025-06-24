// Sunset inc.

#pragma once

namespace Sunset
{
	class AnimationTrack;

	class AnimationClip
	{
		friend class AnimationPlayer;
	public:
		AnimationClip(const std::string& name, float duration, float ticksPerSecond);

		const std::string& GetName() const;
		float GetDuration() const;
		float GetTicksPerSecond() const;

		const std::unordered_map<std::string, std::shared_ptr<AnimationTrack>>& GetTracks() const;

		void AddTrack(const std::string& boneName, const std::shared_ptr<AnimationTrack>& track);

	private:
		std::string m_Name;
		float m_Duration;
		float m_TicksPerSecond;
		std::unordered_map<std::string, std::shared_ptr<AnimationTrack>> m_Tracks;
	};
}
