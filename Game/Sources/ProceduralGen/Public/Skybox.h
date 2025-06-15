// Sunset inc

#pragma once

#include "Entity.h"

namespace Sunset
{
	class SkyBox : public Entity
	{
	public:
		using Entity::Entity;

		virtual void Init() override;
	};
}
