#pragma once

#include "Object.h"
#include "TraceObject.h"

class MainMenu : public Sunset::World
{
public:
	MainMenu();
};

class GameWorld : public Sunset::World
{
public:
	GameWorld();
	virtual ~GameWorld();

	virtual void Update(double deltatime) override;

	virtual void PostRenderObjs() override;

	Sunset::TraceObject draw;
	float spawnTime = 0.f;
	int NbrLife = 3;
};
