// Sunset inc.

#pragma once

#include "Entity.h"

namespace Sunset
{
	class Ground : public Sunset::Entity
	{
	public:
		GENERATED_BODY(Ground);

		virtual void Init() override;
	};
}