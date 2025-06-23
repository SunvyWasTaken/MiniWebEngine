// Sunset inc.

#pragma once

namespace Sunset
{
	template <typename T>
	struct MeshData;

	using StaticMeshData = MeshData<struct StaticVertex>;

	struct AlgoProcedural
	{
		static void PerlinNoise(StaticMeshData& data, float scale = 0.1f, float amplitude = 1.f);
		static void Erosion(StaticMeshData& data, int width, int height);
	};
}
