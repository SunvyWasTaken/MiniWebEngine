// Sunset inc.

#pragma once

#include "Application.h"

namespace Sunset
{
	class EditorEngine : public Engine
	{
	public:
		virtual void LogicLoop(const float deltatime) override;
	};
}
