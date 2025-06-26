// Sunset inc

#pragma once

#include "Entity.h"

namespace Sunset
{
	class SkyBox : public Entity
	{
	public:
		GENERATED_BODY(SkyBox)

		virtual void Init() override;
	};
}
