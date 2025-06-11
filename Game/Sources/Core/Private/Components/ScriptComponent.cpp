// Sunset inc.

#include "Components/ScriptComponent.h"

namespace Sunset
{
	ScriptComponent::ScriptComponent(const FunctionUpdate& functionUpdate, ScriptState* states)
		: m_FuncUpdate(functionUpdate)
		, m_States(states)
	{ }
}
