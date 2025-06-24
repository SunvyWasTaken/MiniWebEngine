// Sunset inc.

#pragma once

namespace Sunset
{
	class AnimationClip;

	class AnimationPlayer
	{
	public:
		AnimationPlayer();

		void Play(const std::shared_ptr<AnimationClip>& clip, bool loop = true);
		void Stop();
		void Update(float deltaTime);

		void GetPose(std::vector<glm::mat4>& outBoneTransforms) const;

		bool IsPlaying() const;
		float GetCurrentTime() const;
		const std::shared_ptr<AnimationClip>& GetCurrentClip() const;

	private:
		std::shared_ptr<AnimationClip> m_CurrentClip;
		float m_CurrentTime;
		bool m_Looping;
		bool m_Playing;
	};
}
