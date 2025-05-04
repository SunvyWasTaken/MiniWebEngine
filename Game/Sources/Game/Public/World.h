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

private:

	template <size_t... Is>
	std::array<Bee*, ShapeModel::Size> create_bee_array(std::index_sequence<Is...>)
	{
		return { ([&]()
		{
			Bee* bee = CreateEntity<Bee>();
			int i = -1 + Is;
			bee->AddPosition({i, -0.7});
			bee->type = std::variant_alternative_t<Is + 1, ShapeModel::Type>{};
			return bee;
		}())... };
	}

public:
	Sunset::TraceObject draw;
	float spawnTime = 0.f;
	int NbrLife = 3;
private:
	std::array<Bee*, ShapeModel::Size> Bees;
};
