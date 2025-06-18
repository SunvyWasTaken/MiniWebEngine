// Sunset inc.

#pragma once

namespace Sunset
{
	class VertexObject;

	class PlaneGen
	{
	public:
		static void Gen(VertexObject& data, float width, float height, int resolutionX, int resolutionY);
		static void ApplyWaveToTerrain(VertexObject& data);
		static void ProcessNormal(VertexObject& data);
	};
}
