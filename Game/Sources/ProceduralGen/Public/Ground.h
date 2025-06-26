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

	class Pig : public Sunset::Entity
	{
	public:
		GENERATED_BODY(Pig);

		virtual void Init() override;

		void Jump(bool pressed) const;
	};
}