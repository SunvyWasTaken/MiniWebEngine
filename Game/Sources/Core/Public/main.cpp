
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

#ifdef __EMSCRIPTEN__
void Mainloop(void* func)
{
	auto* call = static_cast<std::function<void()>*>(func);
	(*call)();
}
#endif // __EMSCRIPTEN__

struct MovementPipe
{
	glm::vec2 dir{-0.5, 0};
	bool IsAlive(const double deltatime)
	{
		lifetime += deltatime;
		return lifetime <= 8.0f;
	}
	double lifetime = 0;
};

void SpawnPipe(Sunset::World& world)
{
	Sunset::Entity UpperPipe = world.create();
	auto& UpperTransComp = world.emplace<Sunset::TransformComponent>(UpperPipe, glm::vec2{2, 0.5});
	UpperTransComp.size = {0.2, 0.5};
	world.emplace<Sunset::RenderObjectComponent>(UpperPipe, new Sunset::Square({1, 2}));
	world.emplace<MovementPipe>(UpperPipe);

	Sunset::Entity UnderPipe = world.create();
	auto& UnderTransComp = world.emplace<Sunset::TransformComponent>(UnderPipe, glm::vec2{2, -0.5 });
	UnderTransComp.size = {0.2, 0.5};
	world.emplace<Sunset::RenderObjectComponent>(UnderPipe, new Sunset::Square({1, 2}));
	world.emplace<MovementPipe>(UnderPipe);
}


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

	Sunset::Entity player = world.create();
	Sunset::TransformComponent& playerTransComp = world.emplace<Sunset::TransformComponent>(player, glm::vec2{-1, 0});
	Sunset::PhysicComponent& playerPhysComp = world.emplace<Sunset::PhysicComponent>(player);
	world.emplace<Sunset::RenderObjectComponent>(player, new Sunset::Square({0, 2}));

	bool IsKeyPress = false;

	double Deltatime = 0.0;

	int32_t frameRate = 1;

	double PreviousTime = window.GetTime();

	float SpawnRate = 0.f;
#ifndef __EMSCRIPTEN__
	gameLayer->Add({ "Localisation", Sunset::GameDataType::Vec2{}, &playerTransComp.location });
	gameLayer->Add({ "Velocity", Sunset::GameDataType::Vec2{}, &playerPhysComp.velocity });
	gameLayer->Add({ "FrameRate", Sunset::GameDataType::Int32{}, &frameRate });
	gameLayer->Add({ "Deltatime", Sunset::GameDataType::Double{}, &Deltatime });
#endif // !__EMSCRIPTEN__

	// Main boucle out for emscripten
	std::function<void()> func = [&]() {
		double CurrentTime = window.GetTime();
		Deltatime = CurrentTime - PreviousTime;
		PreviousTime = CurrentTime;

		frameRate = 1/Deltatime;

		if (Sunset::Inputs::IsKeyPressed(32) && !IsKeyPress)
		{
			IsKeyPress = true;
			playerPhysComp.AddImpulse({0, 1}, 9.81f);
		}
		else if (Sunset::Inputs::IsKeyReleased(32))
		{
			IsKeyPress = false;
		}

		if (SpawnRate < 1.f)
			SpawnRate += Deltatime;
		else
		{
			SpawnRate = 0;
			SpawnPipe(world);
		}

		auto entitys = world.view<Sunset::TransformComponent, Sunset::PhysicComponent>();
		entitys.each([&](Sunset::Entity entity, Sunset::TransformComponent& transComp, Sunset::PhysicComponent& physComp)
			{
				physComp.ApplyPhysic(Deltatime, transComp);
				float alpha = physComp.velocity.y;
				if(alpha > 0)
					alpha = 0;
				else if (alpha < -1)
					alpha = -1;
				transComp.rot = Sunset::Math::lerp(0.f, 45.f, alpha);
			});

		auto pipes = world.view<Sunset::TransformComponent, MovementPipe>();
		pipes.each([&](Sunset::Entity entity, Sunset::TransformComponent& transComp, MovementPipe& movePipe)
			{
				if (!movePipe.IsAlive(Deltatime))
				{
					world.destroy(entity);
					return;
				}

				transComp.location.x += movePipe.dir.x * Deltatime;
				transComp.location.y += movePipe.dir.y * Deltatime;
			});

		auto transComps = world.view<Sunset::TransformComponent>();
		transComps.each([&](Sunset::TransformComponent& transComp)
			{
				
			});

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
