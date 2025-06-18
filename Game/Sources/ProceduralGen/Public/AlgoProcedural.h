// Sunset inc.

#pragma once

namespace Sunset
{
	class VertexObject;

	struct AlgoProcedural
	{
		static void PerlinNoise(VertexObject& data, float scale = 0.1f, float amplitude = 1.f);
		static void Erosion(VertexObject& data, int width, int height);
	};
}
