// Sunset inc.

#pragma once

namespace Sunset
{
	class Object;

	struct AlgoProcedural
	{
		static void PerlinNoise(Object& data, float scale = 0.1f, float amplitude = 1.f);
		static void Erosion(Object& data, int width, int height);
	};
}
