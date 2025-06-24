// Sunset inc.

#pragma once

namespace Sunset
{
	template <typename T, typename... Args>
	T MeshFactory(Args&&... args)
	{
		return T(std::forward<T>(args)...);
	}
}
