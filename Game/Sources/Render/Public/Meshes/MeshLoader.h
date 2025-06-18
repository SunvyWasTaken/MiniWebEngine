// Sunset inc.

#pragma once

namespace Sunset
{
	class VertexObject;

	struct MeshLoader
	{
		static VertexObject LoadMesh(const std::string& path);
	};
}
