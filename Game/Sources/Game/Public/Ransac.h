#pragma once

#include "ShapeDetection.h"

struct Ransac
{
	static ShapeModel::Type ShapeDetection(const std::vector<glm::vec2>& points, int iteration, float threshold);
};
