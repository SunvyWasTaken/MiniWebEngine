#include "ShapeDetection.h"

namespace
{
	double perpendicularDistance(const glm::vec2& pt, const glm::vec2& lineStart, const glm::vec2& lineEnd)
	{
		double dx = lineEnd.x - lineStart.x;
		double dy = lineEnd.y - lineStart.y;

		if (dx == 0 && dy == 0) {
			dx = pt.x - lineStart.x;
			dy = pt.y - lineStart.y;
			return std::sqrt(dx * dx + dy * dy);
		}

		double t = ((pt.x - lineStart.x) * dx + (pt.y - lineStart.y) * dy) / (dx * dx + dy * dy);
		double projX = lineStart.x + t * dx;
		double projY = lineStart.y + t * dy;
		dx = pt.x - projX;
		dy = pt.y - projY;
		return std::sqrt(dx * dx + dy * dy);
	}
}

void ShapeDetection::DouglasPeuckerFunction(const std::vector<glm::vec2>& points, double epsilon, std::vector<glm::vec2>& out)
{
	if (points.size() < 4)
	{
		out = points;
		return;
	}

	double dmax = 0;
	size_t index = 0;
	for (size_t i = 2; i < points.size() - 1; ++i)
	{
		double distance = perpendicularDistance(points[i], points[0], points.back());
		if (distance > dmax)
		{
			index = i;
			dmax = distance;
		}
	}

	if (dmax > epsilon)
	{
		std::vector<glm::vec2> recResults1;
		std::vector<glm::vec2> recResults2;
		std::vector<glm::vec2> firstLine(points.begin(), points.begin() + index + 1);
		std::vector<glm::vec2> lastLine(points.begin() + index, points.end());
		DouglasPeuckerFunction(firstLine, epsilon, recResults1);
		DouglasPeuckerFunction(lastLine, epsilon, recResults2);

		// Combiner les résultats
		out.assign(recResults1.begin(), recResults1.end() - 1);
		out.insert(out.end(), recResults2.begin(), recResults2.end());
	}
	else 
	{
		// Si la distance maximale est inférieure à epsilon, conserver les extrémités
		out.clear();
		out.push_back(points[0]);
		out.push_back(points.back());
	}
}

ShapeModel::Type ShapeDetection::Detect(const std::vector<glm::vec2>& points)
{
	std::vector<glm::vec2> out;
	ShapeDetection::DouglasPeuckerFunction(points, 0.05f, out);

	ShapeModel::Type model;
	if (out.size() >= 8)
		model = ShapeModel::Cercle{};
	else if(out.size() == 5)
		model = ShapeModel::Square{};
	else if(out.size() == 4)
		model = ShapeModel::Triangle{};

	return model;
}
