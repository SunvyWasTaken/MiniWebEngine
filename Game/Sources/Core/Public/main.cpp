
#include "Camera.h"
#include "Entitys.h"
#ifndef __EMSCRIPTEN__
#include "Layer.h"
#endif
#include "Ransac.h"
#include "Render.h"
#include "WorldManager.h"
#include "World.h"

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

int main()
{
	Sunset::Render window;

#ifndef __EMSCRIPTEN__
	std::shared_ptr<Sunset::Layer> gameLayer = std::make_shared<Sunset::Layer>(Sunset::Layer{"Player Data"});
	Sunset::LayerContainer::Get().Add(gameLayer);
#endif

	Sunset::WorldManager<MainMenu, GameWorld> worldManager;

	std::shared_ptr<Sunset::Camera> cam = std::make_shared<Sunset::Camera>();

	double Deltatime = 0.0;

	int32_t frameRate = 1;

	double PreviousTime = window.GetTime();

	float SpawnRate = 0.f;
#ifndef __EMSCRIPTEN__
	gameLayer->Add({ "Deltatime", Sunset::GameDataType::Double{}, &Deltatime });
	gameLayer->Add({ "FrameRate", Sunset::GameDataType::Int32{}, &frameRate });
	gameLayer->Add({ "Main Menu", Sunset::GameDataType::Button{[&]()
	{
		worldManager.LoadWorld<MainMenu>();
	}}});
	gameLayer->Add({ "Game World", Sunset::GameDataType::Button{[&]()
	{
		worldManager.LoadWorld<GameWorld>();
	}} });
#endif // !__EMSCRIPTEN__

	// Main boucle out for emscripten
	std::function<void()> func = [&]() {
		double CurrentTime = window.GetTime();
		Deltatime = CurrentTime - PreviousTime;
		PreviousTime = CurrentTime;

		frameRate = 1 / Deltatime;

		worldManager.Update(Deltatime);

		cam->Update(Deltatime);

		// Render
		window.Begin(cam);
#ifndef __EMSCRIPTEN__
		Sunset::LayerContainer::Render();
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
