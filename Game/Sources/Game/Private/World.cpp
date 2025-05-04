#include "Entitys.h"
#include "Inputs.h"
#include "Layer.h"
#include "Render.h"
#include "ShapeDetection.h"
#include "World.h"

namespace
{
	std::shared_ptr<Sunset::Layer> GameLayer = nullptr;
	//std::vector<float> xList;
	//std::vector<float> yList;
	//std::vector<float> xListBis;
	//std::vector<float> yListBis;
	//size_t xSize = 0;
	//size_t xBisSize = 0;
	//size_t ySize = 0;
	//size_t yBisSize = 0;
	//constexpr auto ClearList = [&](){ xList.clear(); yList.clear(); xListBis.clear(); yListBis.clear(); };
}

MainMenu::MainMenu()
	: Sunset::World()
{
	LOG("Main Menu create");
}


GameWorld::GameWorld()
	: Sunset::World()
{
	LOG("game World create");
	if (!GameLayer)
	{
		GameLayer = std::make_shared<Sunset::Layer>("Game world");
		Sunset::LayerContainer::Get().Add(GameLayer);
		GameLayer->Add({"LifePoints", Sunset::GameDataType::Int32{}, &NbrLife});
		//GameLayer->Add({ "X:", Sunset::GameDataType::GraphLine{&xList} });
		//GameLayer->Add({ "size", Sunset::GameDataType::Int32{}, &xSize});
		//GameLayer->Add({ "XBis:", Sunset::GameDataType::GraphLine{&xListBis} });
		//GameLayer->Add({ "size", Sunset::GameDataType::Int32{}, &xBisSize});
		//GameLayer->Add({ "Y:", Sunset::GameDataType::GraphLine{&yList} });
		//GameLayer->Add({ "size", Sunset::GameDataType::Int32{}, &ySize});
		//GameLayer->Add({ "YBis:", Sunset::GameDataType::GraphLine{&yListBis} });
		//GameLayer->Add({ "size", Sunset::GameDataType::Int32{}, &yBisSize});
		//GameLayer->Add({ "Clear", Sunset::GameDataType::Button{ClearList}});
	}
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

	//if (Sunset::Inputs::IsKey(32))
	//{
	//	ClearList();
	//}

	if (Sunset::Inputs::IsMouseButton(0, Sunset::Inputs::State::Hold{}))
	{
		glm::vec2 ndc;
		glm::vec2 point = Sunset::Inputs::GetMouseScreenPosition();
		ndc.x = (point.x / Sunset::Render::GetWidth()) * 2.0f - 1.0f;
		ndc.y = 1.0f - (point.y / Sunset::Render::GetHeight()) * 2.0f;
		draw.AddPoints(ndc);
		//xList.push_back(ndc.x);
		//yList.push_back(ndc.y);
		//xSize = xList.size();
		//ySize = yList.size();
	}
	else if(draw)
	{
		ShapeModel::Type shape = ShapeDetection::Detect(draw);
		std::vector<glm::vec2> out;
		//ShapeDetection::DouglasPeuckerFunction(draw, 0.05f, out);
		//for (auto& i : out)
		//{
		//	xListBis.push_back(i.x);
		//	yListBis.push_back(i.y);
		//}
		//xBisSize = xListBis.size();
		//yBisSize = yListBis.size();
		std::visit(Sunset::overload
		{
			[&](std::monostate&)
			{
				LOG("unknow");
			},
			[&](ShapeModel::Cercle&)
			{
				LOG("Cercle");
			},
			[&](ShapeModel::Triangle&)
			{
				LOG("Triangle")
			},
			[&](ShapeModel::Square&)
			{
				LOG("Square")
			}
		}, shape);
		draw.Clear();
	}
}

void GameWorld::PostRenderObjs()
{
	draw.Draw();
}
