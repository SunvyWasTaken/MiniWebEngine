// Sunset inc.

#pragma once

namespace Sunset
{
	template <typename T>
	struct VertexObject;

	using StaticVertices = VertexObject<struct Vertex>;

	class PlaneGen
	{
	public:
		static void Gen(StaticVertices& data, float width, float height, int resolutionX, int resolutionY);
		static void ApplyWaveToTerrain(StaticVertices& data);
		static void ProcessNormal(StaticVertices& data);
	};
}
