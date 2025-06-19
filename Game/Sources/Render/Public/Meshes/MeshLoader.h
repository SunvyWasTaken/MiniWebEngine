// Sunset inc.

#pragma once

namespace Sunset
{
	template <typename VertexType>
	class VertexObject;

	using VerticeType = std::variant<VertexObject<struct Vertex>, VertexObject<struct VertexSkeletal>>;

	struct MeshLoader
	{
		static void LoadMesh(const std::string& path, VerticeType& vertices, float& m_ImportScale);
	};
}
