// Sunset inc.

#pragma once

namespace Sunset
{
	class Object;

	class PlaneGen
	{
	public:
		static void Gen(Object& data, float width, float height, int resolutionX, int resolutionY);
		static void ApplyWaveToTerrain(Object& data);
		static void ProcessNormal(Object& data);
	};
}
