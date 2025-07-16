// Sunset inc.

#pragma once

#include "BaseComponent.h"

namespace Sunset
{
	class ScriptComponent : public BaseComponent
	{
		friend class Scene;
		using FunctionUpdate = std::function<void(float)>;

	public:
		explicit ScriptComponent(const FunctionUpdate& functionUpdate);

		FunctionUpdate m_FuncUpdate;
	};
}
