// Sunset inc.

#pragma once

#include "Entity.h"

namespace Sunset
{
	class Ground : public Sunset::Entity
	{
	public:
		using Sunset::Entity::Entity;

		virtual void Init() override;
	};
}