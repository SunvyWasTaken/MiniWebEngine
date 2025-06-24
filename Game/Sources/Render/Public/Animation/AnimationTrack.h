// Sunset inc.

#pragma once

namespace Sunset
{
	struct KeyPosition
	{
		float time;
		glm::vec3 position;
	};

	struct KeyRotation
	{
		float time;
		glm::quat rotation;
	};

	struct KeyScale
	{
		float time;
		glm::vec3 scale;
	};

	class AnimationTrack
	{
	public:
		AnimationTrack();

		void AddPositionKey(const KeyPosition& key);
		void AddRotationKey(const KeyRotation& key);
		void AddScaleKey(const KeyScale& key);

		glm::vec3 InterpolatePosition(float time) const;
		glm::quat InterpolateRotation(float time) const;
		glm::vec3 InterpolateScale(float time) const;

		const std::vector<KeyPosition>& GetPositionKeys() const;
		const std::vector<KeyRotation>& GetRotationKeys() const;
		const std::vector<KeyScale>& GetScaleKeys() const;

	private:
		std::vector<KeyPosition> m_PositionKeys;
		std::vector<KeyRotation> m_RotationKeys;
		std::vector<KeyScale> m_ScaleKeys;
	};
}
