// Sunset inc.

#include "Animation/AnimationTrack.h"

namespace Sunset
{

	AnimationTrack::AnimationTrack()
		: m_PositionKeys()
		, m_RotationKeys()
		, m_ScaleKeys()
	{

	}

	void AnimationTrack::AddPositionKey(const KeyPosition& key)
	{
		m_PositionKeys.emplace_back(key);
	}

	void AnimationTrack::AddRotationKey(const KeyRotation& key)
	{
		m_RotationKeys.emplace_back(key);
	}

	void AnimationTrack::AddScaleKey(const KeyScale& key)
	{
		m_ScaleKeys.emplace_back(key);
	}

	glm::vec3 AnimationTrack::InterpolatePosition(float time) const
	{
		const auto& keys = m_PositionKeys;
		if (keys.empty())
			return glm::vec3(0.0f);

		if (time <= keys.front().time)
			return keys.front().position;
		if (time >= keys.back().time)
			return keys.back().position;

		auto it = std::lower_bound(
			keys.begin(), keys.end(), time,
			[](const KeyPosition& key, float t) { return key.time < t; });

		size_t idx = std::distance(keys.begin(), it);
		const KeyPosition& next = keys[idx];
		const KeyPosition& prev = keys[idx - 1];

		float t = (time - prev.time) / (next.time - prev.time);
		return glm::mix(prev.position, next.position, t);
	}

	glm::quat AnimationTrack::InterpolateRotation(float time) const
	{
		const auto& keys = m_RotationKeys;
		if (keys.empty())
			return glm::quat(1, 0, 0, 0);

		if (time <= keys.front().time)
			return keys.front().rotation;
		if (time >= keys.back().time)
			return keys.back().rotation;

		auto it = std::lower_bound(
			keys.begin(), keys.end(), time,
			[](const KeyRotation& key, float t) { return key.time < t; });

		size_t idx = std::distance(keys.begin(), it);
		const KeyRotation& next = keys[idx];
		const KeyRotation& prev = keys[idx - 1];

		float t = (time - prev.time) / (next.time - prev.time);
		return glm::slerp(prev.rotation, next.rotation, t);
	}

	glm::vec3 AnimationTrack::InterpolateScale(float time) const
	{
		const auto& keys = m_ScaleKeys;
		if (keys.empty())
			return glm::vec3(1.0f);

		if (time <= keys.front().time)
			return keys.front().scale;
		if (time >= keys.back().time)
			return keys.back().scale;

		auto it = std::lower_bound(
			keys.begin(), keys.end(), time,
			[](const KeyScale& key, float t) { return key.time < t; });

		size_t idx = std::distance(keys.begin(), it);
		const KeyScale& next = keys[idx];
		const KeyScale& prev = keys[idx - 1];

		float t = (time - prev.time) / (next.time - prev.time);
		return glm::mix(prev.scale, next.scale, t);
	}

	const std::vector<Sunset::KeyPosition>& AnimationTrack::GetPositionKeys() const
	{
		return m_PositionKeys;
	}

	const std::vector<Sunset::KeyRotation>& AnimationTrack::GetRotationKeys() const
	{
		return m_RotationKeys;
	}

	const std::vector<Sunset::KeyScale>& AnimationTrack::GetScaleKeys() const
	{
		return m_ScaleKeys;
	}

}
