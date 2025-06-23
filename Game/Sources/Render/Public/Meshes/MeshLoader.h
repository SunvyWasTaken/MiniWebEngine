// Sunset inc.

#pragma once

namespace Sunset
{
	class StaticMesh;
	class SkeletalMesh;

	using Meshes = std::variant<StaticMesh, SkeletalMesh>;

	struct MeshLoader
	{
		static [[nodiscard("You forgot to store the loaded static mesh")]] std::shared_ptr<Meshes> LoadStaticMesh(const std::string& path) noexcept;
		static [[nodiscard("You forgot to store the loaded skeletal mesh")]] std::shared_ptr<Meshes> LoadSkeletalMesh(const std::string& path) noexcept;
	};
}
