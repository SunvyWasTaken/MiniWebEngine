// Sunset inc.

#pragma once

namespace Sunset
{
	template <typename T>
	struct VertexObject;

	using StaticVertices = VertexObject<struct Vertex>;

	struct AlgoProcedural
	{
		static void PerlinNoise(StaticVertices& data, float scale = 0.1f, float amplitude = 1.f);
		static void Erosion(StaticVertices& data, int width, int height);
	};
}
