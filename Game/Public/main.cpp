
#include "Camera.h"
#include "Inputs.h"
#ifndef __EMSCRIPTEN__
#include "Layer.h"
#endif
#include "Object.h"
#include "Render.h"
#include "RenderObject.h"

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#endif // __EMSCRIPTEN__


#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <imgui.h>

#include <array>
#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <random>

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
	Sunset::LayerContainer layerContainer;
	std::shared_ptr<Sunset::Layer> gameLayer = std::make_shared<Sunset::Layer>(Sunset::Layer{"Player Data"});
	layerContainer(gameLayer);
#endif

	Sunset::World world;

	std::shared_ptr<Sunset::Camera> cam = std::make_shared<Sunset::Camera>();

	double Deltatime = 0.0;

	int32_t frameRate = 1;

	// Main boucle out for emscripten
	std::function<void()> func = [&]() {

		double CurrentTime = window.GetTime();
		Deltatime = CurrentTime - PreviousTime;
		PreviousTime = CurrentTime;

		frameRate = 1/Deltatime;

		cam->Update(Deltatime);

		// Render
		window.Begin(cam);
#ifndef __EMSCRIPTEN__
		layerContainer.Render();
#endif

		auto rendables = world.view<Sunset::TransformComponent, Sunset::RenderObjectComponent>();
		rendables.each([&](Sunset::TransformComponent& transform, Sunset::RenderObjectComponent& RenderObj)
		{
			window.RenderObj(transform, *(RenderObj.data));
		});

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
