#include "Entitys.h"
#include "Inputs.h"
#include "Layer.h"
#include "Render.h"
#include "ShapeDetection.h"
#include "World.h"

namespace
{
	std::shared_ptr<Sunset::Layer> GameLayer = nullptr;
}

MainMenu::MainMenu()
	: Sunset::World()
{
	// Ici faudra que je fasse en sorte que le jeux écrive du texte pour que je puisse mettre un presse to start.
}


GameWorld::GameWorld()
	: Sunset::World()
{
	if (!GameLayer)
	{
		GameLayer = std::make_shared<Sunset::Layer>("Game world");
		Sunset::LayerContainer::Get().Add(GameLayer);
		GameLayer->Add({"LifePoints", Sunset::GameDataType::Int32{}, &NbrLife});
	}

	Bees = create_bee_array(std::make_index_sequence<ShapeModel::Size>{});
}

GameWorld::~GameWorld()
{
	GameLayer.reset();
}

void GameWorld::Update(double deltatime)
{
	Sunset::World::Update(deltatime);

	spawnTime += deltatime;
	if (spawnTime >= 2.f)
	{
		//CreateEntity<Ennemy>();
		spawnTime = 0;
	}

	if (Sunset::Inputs::IsMouseButton(0, Sunset::Inputs::State::Hold{}))
	{
		glm::vec2 ndc;
		glm::vec2 point = Sunset::Inputs::GetMouseScreenPosition();
		ndc.x = (point.x / Sunset::Render::GetWidth()) * 2.0f - 1.0f;
		ndc.y = 1.0f - (point.y / Sunset::Render::GetHeight()) * 2.0f;
		draw.AddPoints(ndc);
	}
	else if(draw)
	{
		ShapeModel::Type shape = ShapeDetection::Detect(draw);

		std::visit(Sunset::overload
		{
			[&](std::monostate&)
			{
				LOG("unknow");
			},
			[&](ShapeModel::Triangle&)
			{
				LOG("Triangle")
				Bees[0]->Attack(std::vector<Ennemy>());
			},
			[&](ShapeModel::Square&)
			{
				LOG("Square")
				Bees[1]->Attack(std::vector<Ennemy>());
			},
			[&](ShapeModel::Cercle&)
			{
				LOG("Cercle");
				Bees[2]->Attack(std::vector<Ennemy>());
			}
		}, shape);
		draw.Clear();
	}
}

void GameWorld::PostRenderObjs()
{
	draw.Draw();
}
