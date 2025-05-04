#pragma once

namespace Sunset
{
	class TraceObject
	{
		using PointsList = std::vector<glm::vec2>;
	public:
		TraceObject();
		~TraceObject();

		void AddPoints(const glm::vec2& point);

		void Draw();

		void Clear();

		operator PointsList();

		operator bool();

	private:
		PointsList points;
	};
}
