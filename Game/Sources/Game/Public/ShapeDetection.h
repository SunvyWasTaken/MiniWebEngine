#pragma once

namespace ShapeModel
{
	struct BaseLine
	{
		std::vector<glm::vec2> data;
	};
	struct Triangle : public BaseLine {};
	struct Square : public BaseLine {};
	struct Cercle
	{
		glm::vec2 center;
		float radius;
	};
	using Type = std::variant<std::monostate, Triangle, Square, Cercle>;
	constexpr size_t Size = std::variant_size<Type>::value - 1;
};

struct ShapeDetection
{
	/// <summary>
	/// Reduit le nbr de points dans une courbe pour simplifier le traitement des points
	/// </summary>
	/// <param name="points">List totale des points de la courbe</param>
	/// <param name="epsilon">Valeur de tolerance pour la difference entre les points (ex: valeur=1 supprime les points qui ne contribuent pas bcp à la forme de la courbe)</param>
	/// <param name="out">Vecteur des points representant la courbe plus courts</param>
	static void DouglasPeuckerFunction(const std::vector<glm::vec2>& points, double epsilon, std::vector<glm::vec2>& out);

	static ShapeModel::Type Detect(const std::vector<glm::vec2>& points);
};
