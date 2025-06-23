// Sunset inc.

#pragma once

namespace Sunset
{
	template <typename T>
	struct MeshData;

	using StaticMeshData = MeshData<struct StaticVertex>;

	class PlaneGen
	{
	public:
		static void Gen(StaticMeshData& data, float width, float height, int resolutionX, int resolutionY);
		static void ApplyWaveToTerrain(StaticMeshData& data);
		static void ProcessNormal(StaticMeshData& data);
	};
}
