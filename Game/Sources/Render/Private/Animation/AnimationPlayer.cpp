// Sunset inc.

#include "Animation/AnimationPlayer.h"
#include "Animation/AnimationClip.h"
#include "Animation/AnimationTrack.h"

#include <glm/gtc/quaternion.hpp>

namespace Sunset
{

	AnimationPlayer::AnimationPlayer()
		: m_CurrentClip(nullptr)
		, m_CurrentTime(0.f)
		, m_Looping(true)
		, m_Playing(false)
	{

	}

	void AnimationPlayer::Play(const std::shared_ptr<AnimationClip>& clip, bool loop /*= true*/)
	{
		m_CurrentClip = clip;
		m_Looping = loop;
		m_Playing = true;
	}

	void AnimationPlayer::Stop()
	{
		m_Playing = false;
		m_CurrentTime = 0.f;
	}

	void AnimationPlayer::Update(float deltaTime)
	{
		// Only update if an animation is playing
		if (!m_Playing || !m_CurrentClip)
			return;

		// Advance the animation time (in ticks)
		float ticksPerSecond = m_CurrentClip->GetTicksPerSecond();
		float duration = m_CurrentClip->GetDuration();
		float deltaTicks = deltaTime * (ticksPerSecond > 0.0f ? ticksPerSecond : 25.0f);

		m_CurrentTime += deltaTicks;

		// Handle looping or stop at the end
		if (m_CurrentTime > duration)
		{
			if (m_Looping)
				m_CurrentTime = fmod(m_CurrentTime, duration);
			else
			{
				m_CurrentTime = duration;
				m_Playing = false;
			}
		}
	}

	void AnimationPlayer::GetPose(std::vector<glm::mat4>& outBoneTransforms) const
	{
		// Clear the output
		outBoneTransforms.clear();

		if (!m_CurrentClip)
			return;

		// For each track (bone), interpolate position, rotation, scale and build the local transform
		const auto& tracks = m_CurrentClip->m_Tracks;
		outBoneTransforms.reserve(tracks.size());

		for (const auto& track : tracks)
		{
			if (!track)
			{
				outBoneTransforms.emplace_back(glm::mat4(1.f));
				continue;
			}

			glm::vec3 pos = track->InterpolatePosition(m_CurrentTime);
			glm::quat rot = track->InterpolateRotation(m_CurrentTime);
			glm::vec3 scale = track->InterpolateScale(m_CurrentTime);

			// Build the transformation matrix: T * R * S
			glm::mat4 T = glm::translate(glm::mat4(1.0f), pos);
			glm::mat4 R = glm::mat4_cast(rot);
			glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);

			glm::mat4 localTransform = T * R * S;
			outBoneTransforms.emplace_back(localTransform);
		}
	}

	bool AnimationPlayer::IsPlaying() const
	{
		return m_Playing;
	}

	float AnimationPlayer::GetCurrentTime() const
	{
		return m_CurrentTime;
	}

	const std::shared_ptr<Sunset::AnimationClip>& AnimationPlayer::GetCurrentClip() const
	{
		return m_CurrentClip;
	}

}
