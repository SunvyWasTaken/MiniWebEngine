#include "Ransac.h"

namespace
{
	std::random_device rd;
	std::mt19937 gen(rd());

	using PointsData = std::array<glm::vec2, 4>;

	ShapeModel::Type estimateCircle(const PointsData& p)
	{
		// Calcul des milieux des segments
		glm::vec2 midAB = (p[0] + p[1]) * 0.5f;
		glm::vec2 midBC = (p[1] + p[2]) * 0.5f;

		// Vecteurs directeurs des segments
		glm::vec2 dirAB = p[1] - p[0];
		glm::vec2 dirBC = p[2] - p[1];

		// Vecteurs normaux (perpendiculaires)
		glm::vec2 normAB(-dirAB.y, dirAB.x);
		glm::vec2 normBC(-dirBC.y, dirBC.x);

		// Résolution du système linéaire pour trouver l'intersection des médiatrices
		// midAB + t1 * normAB = midBC + t2 * normBC
		// => t1 * normAB - t2 * normBC = midBC - midAB

		float denominator = normAB.x * normBC.y - normAB.y * normBC.x;
		if (std::abs(denominator) < 1e-6f) {
			// Les médiatrices sont parallèles, les points sont alignés
			return {}; // Retourner un modèle vide ou invalide
		}

		glm::vec2 delta = midBC - midAB;
		float t1 = (delta.x * normBC.y - delta.y * normBC.x) / denominator;
		glm::vec2 center = midAB + t1 * normAB;
		float radius = glm::length(center - p[0]);

		ShapeModel::Type model = ShapeModel::Cercle{center, radius};
		return model;
	}

	ShapeModel::Type estimateTriangle(const PointsData& p)
	{
		ShapeModel::Type model = ShapeModel::Triangle{};
		std::get<ShapeModel::Triangle>(model).data = {p[0], p[1], p[2]};
		return model;
	}

	ShapeModel::Type estimateSquare(PointsData& points)
	{
		// Définir une fonction de comparaison pour glm::vec2
		auto vec2_less = [](const glm::vec2& a, const glm::vec2& b) {
			if (a.x != b.x) return a.x < b.x;
			return a.y < b.y;
			};

		// Trier les points pour commencer avec une permutation déterminée
		std::sort(points.begin(), points.end(), vec2_less);

		do {
			float d01 = glm::length(points[0] - points[1]);
			float d12 = glm::length(points[1] - points[2]);
			float d23 = glm::length(points[2] - points[3]);
			float d30 = glm::length(points[3] - points[0]);

			float diag1 = glm::length(points[0] - points[2]);
			float diag2 = glm::length(points[1] - points[3]);

			// Vérifier l'égalité des côtés et des diagonales
			if (std::abs(d01 - d12) < 1e-3f &&
				std::abs(d12 - d23) < 1e-3f &&
				std::abs(d23 - d30) < 1e-3f &&
				std::abs(diag1 - diag2) < 1e-3f) {
				// Vérifier les angles droits
				glm::vec2 v0 = points[1] - points[0];
				glm::vec2 v1 = points[2] - points[1];
				if (std::abs(glm::dot(glm::normalize(v0), glm::normalize(v1))) < 1e-3f) {
					ShapeModel::Type model = ShapeModel::Square{};
					std::get<ShapeModel::Square>(model).data = {points[0], points[1], points[2], points[3]};
					return model;
				}
			}
		} while (std::next_permutation(points.begin(), points.end(), vec2_less));

		return {}; // Retourner un modèle vide ou invalide
	}

	float pointToSegmentDistance(const glm::vec2& p, const glm::vec2& a, const glm::vec2& b) {
		glm::vec2 ab = b - a;
		float ab_squared = glm::dot(ab, ab);
		if (ab_squared == 0.0f) {
			return glm::length(p - a);
		}
		float t = glm::dot(p - a, ab) / ab_squared;
		t = std::clamp(t, 0.0f, 1.0f);
		glm::vec2 projection = a + t * ab;
		return glm::length(p - projection);
	}

	int countInliers(const ShapeModel::Type& obj, const std::vector<glm::vec2>& points, float threshold) {
		int inliers = 0;
		for (const auto& point : points)
		{
			float error = std::numeric_limits<float>::max();
			std::visit(Sunset::overload
			{
				[&](const std::monostate&) {},
				[&](const ShapeModel::Cercle& model)
				{
					error = std::abs(glm::length(point - model.center) - model.radius);
				},
				[&](const ShapeModel::Triangle& model)
				{
					if (model.data.empty())
						return;

					error = std::min({
					pointToSegmentDistance(point, model.data[0], model.data[1]),
					pointToSegmentDistance(point, model.data[1], model.data[2]),
					pointToSegmentDistance(point, model.data[2], model.data[0])
					});
				},
				[&](const ShapeModel::Square& model)
				{
					if(model.data.empty())
						return;

					error = std::min({
					pointToSegmentDistance(point, model.data[0], model.data[1]),
					pointToSegmentDistance(point, model.data[1], model.data[2]),
					pointToSegmentDistance(point, model.data[2], model.data[3]),
					pointToSegmentDistance(point, model.data[3], model.data[0])
					});
				}
			}, obj);

			if (error < threshold) {
				++inliers;
			}
		}
		return inliers;
	}
}

ShapeModel::Type Ransac::ShapeDetection(const std::vector<glm::vec2>& points, int iteration, float threshold)
{
	SCOPE_PERF("Ransac Detection")
	ShapeModel::Type bestModel;
	int bestInliers = 0;

	for (int i = 0; i < iteration; ++i)
	{
		std::uniform_int_distribution<> dis(0, points.size() - 1);
		PointsData ps;
		for (auto& p : ps)
		{
			int i = dis(gen);
			p = points[i];
		}

		// Estimation des modèles
		ShapeModel::Type circleModel = estimateCircle(ps);
		ShapeModel::Type triangleModel = estimateTriangle(ps);
		ShapeModel::Type squareModel = estimateSquare(ps);

		// Comptage des inliers
		int circleInliers = countInliers(circleModel, points, threshold);
		int triangleInliers = countInliers(triangleModel, points, threshold);
		int squareInliers = countInliers(squareModel, points, threshold);

		// Mise à jour du meilleur modèle
		if (circleInliers > bestInliers) {
			bestInliers = circleInliers;
			bestModel = circleModel;
		}
		if (triangleInliers > bestInliers) {
			bestInliers = triangleInliers;
			bestModel = triangleModel;
		}
		if (squareInliers > bestInliers) {
			bestInliers = squareInliers;
			bestModel = squareModel;
		}
	}
	return bestModel;
}
