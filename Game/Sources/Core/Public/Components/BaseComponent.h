// Sunset inc.

#pragma once

namespace Sunset
{
	class Entity;

	class BaseComponent
	{
		friend Entity;
	public:
		BaseComponent() = default;
		virtual ~BaseComponent() = default;
	protected:
		Entity* owner = nullptr;
	};
}
