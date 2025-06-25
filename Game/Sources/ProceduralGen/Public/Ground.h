// Sunset inc.

#pragma once

#include "Entity.h"

namespace Sunset
{
	class Ground : public Sunset::Entity
	{
	public:
		GENERATED_BODY;

		virtual void Init() override;
	};

	class Pig : public Sunset::Entity
	{
	public:
		GENERATED_BODY;

		virtual void Init() override;

		void Jump();
	};
}