
#include "Camera.h"
#include "Components.h"
#include "Inputs.h"
#ifndef __EMSCRIPTEN__
#include "Layer.h"
#endif
#include "Object.h"
#include "Quadtree.h"
#include "Render.h"
#include "RenderObject.h"
#include "WorldManager.h"

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#endif // __EMSCRIPTEN__

#ifdef __EMSCRIPTEN__
void Mainloop(void* func)
{
	auto* call = static_cast<std::function<void()>*>(func);
	(*call)();
}
#endif // __EMSCRIPTEN__

class MainMenu : public Sunset::World
{
public:
	MainMenu() : Sunset::World()
	{
		LOG("Main Menu create");
	}
};

class Player : public Sunset::Entity
{
public:

	BODY(Player)

	virtual void Begin() override
	{
		transComp = &AddComponent<Sunset::TransformComponent>(glm::vec2{0, 0});
		AddComponent<Sunset::RenderObjectComponent>(new Sunset::Square({0, 3}));
	}

	virtual void Update(double deltatime)
	{
		if (Sunset::Inputs::IsKey(87, Sunset::Inputs::State::Hold{}))
		{
			AddPosition({ 0.f, PlayerSpeed * deltatime });
		}
		if (Sunset::Inputs::IsKey(83, Sunset::Inputs::State::Hold{}))
		{
			AddPosition({ 0.f, -PlayerSpeed * deltatime });
		}
		if (Sunset::Inputs::IsKey(65, Sunset::Inputs::State::Hold{}))
		{
			AddPosition({ -PlayerSpeed * deltatime, 0.f });
		}
		if (Sunset::Inputs::IsKey(68, Sunset::Inputs::State::Hold{}))
		{
			AddPosition({ PlayerSpeed * deltatime, 0.f });
		}
	}

	void AddPosition(const glm::vec2& pos)
	{
		transComp->location += pos;
	}

	Sunset::TransformComponent* transComp;
	float PlayerSpeed = 0.5f;
};

class GameWorld : public Sunset::World
{
public:
	GameWorld() : Sunset::World()
	{
		LOG("game World create");
		playerRef = CreateEntity<Player>();
	}

	Player* playerRef;
};


int main()
{
	Sunset::Render window;

#ifndef __EMSCRIPTEN__
	Sunset::LayerContainer layerContainer;
	std::shared_ptr<Sunset::Layer> gameLayer = std::make_shared<Sunset::Layer>(Sunset::Layer{"Player Data"});
	layerContainer(gameLayer);
#endif

	Sunset::WorldManager<MainMenu, GameWorld> worldManager;

	std::shared_ptr<Sunset::Camera> cam = std::make_shared<Sunset::Camera>();


	bool IsKeyPress = false;

	double Deltatime = 0.0;

	int32_t frameRate = 1;

	double PreviousTime = window.GetTime();

	float SpawnRate = 0.f;
#ifndef __EMSCRIPTEN__
	gameLayer->Add({ "Deltatime", Sunset::GameDataType::Double{}, &Deltatime });
	gameLayer->Add({ "FrameRate", Sunset::GameDataType::Int32{}, &frameRate });
#endif // !__EMSCRIPTEN__

	// Main boucle out for emscripten
	std::function<void()> func = [&]() {
		double CurrentTime = window.GetTime();
		Deltatime = CurrentTime - PreviousTime;
		PreviousTime = CurrentTime;

		frameRate = 1 / Deltatime;

		if (Sunset::Inputs::IsKey(70))
		{
			worldManager.LoadWorld<GameWorld>();
		}
		else if (Sunset::Inputs::IsKey(71))
		{
			worldManager.LoadWorld<MainMenu>();
		}

		worldManager.Update(Deltatime);

		cam->Update(Deltatime);

		// Render
		window.Begin(cam);
#ifndef __EMSCRIPTEN__
		layerContainer.Render();
#endif
	
		worldManager.RenderObj(&window);

		window.End();
	};

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop_arg(Mainloop, &func, 0, true);
#else
	while (!window)
	{
		func();
	}
#endif

}
