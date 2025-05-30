#pragma once

#include "Object.h"
#include "TraceObject.h"
#include "WorldManager.h"

constexpr size_t NbrEnnemy = 10;

class MainMenu : public Sunset::World
{
public:
	MainMenu();
};

class GameWorld : public Sunset::World
{
	using EnnemyList = std::array<Ennemy*, NbrEnnemy>; 
public:
	GameWorld();
	virtual ~GameWorld();

	virtual void Update(double deltatime) override;

	virtual void PostRenderObjs() override;

	void LostLife();

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

	template <size_t... Is>
	EnnemyList CreateEnnemys(std::index_sequence<Is...>)
	{
		return { ([&]()
		{
			Ennemy* curr = CreateEntity<Ennemy>();
			curr->AddPosition({0, 1});
			Is;
			return curr;
		}())... };
	}

public:
	Sunset::TraceObject draw;
	float spawnTime = 0.f;
	int NbrLife = 3;
private:
	std::array<Bee*, ShapeModel::Size> bees;
	EnnemyList ennemys;
};

extern Sunset::WorldManager<MainMenu, GameWorld> worlds;
