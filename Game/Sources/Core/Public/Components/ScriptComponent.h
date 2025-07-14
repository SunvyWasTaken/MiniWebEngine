// Sunset inc.

#pragma once

#include "BaseComponent.h"

namespace entt
{
	enum class entity : uint32_t;
}

namespace Sunset
{
	class ScriptComponent : public BaseComponent
	{
		friend class Scene;
		using FunctionUpdate = std::function<void(float)>;

	public:
		ScriptComponent(const FunctionUpdate& functionUpdate);

		FunctionUpdate m_FuncUpdate;
	};
}
