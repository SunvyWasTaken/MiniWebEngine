// Sunset inc.

#pragma once

#include "BaseComponent.h"

namespace entt
{
	enum class entity : uint32_t;
}

namespace Sunset
{
	struct ScriptState : public BaseComponent
	{
		virtual ~ScriptState() = default;
	};

	class ScriptComponent
	{
		friend class Scene;
		using FunctionUpdate = std::function<void(entt::entity, const float)>;
		using States = std::unique_ptr<ScriptState>;

	public:
		ScriptComponent(const FunctionUpdate& functionUpdate, ScriptState* states);

		FunctionUpdate m_FuncUpdate;
		
		States m_States;
	};
}
