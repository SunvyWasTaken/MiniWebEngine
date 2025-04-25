
#include "Camera.h"
#include "Inputs.h"
#include "Layer.h"
#include "Object.h"
#include "Render.h"
#include "RenderObject.h"

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#endif // __EMSCRIPTEN__


#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <imgui.h>

#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <random>

std::ostream& operator<<(std::ostream& io, const glm::vec2& vec)
{
	return io << "x:" << vec.x << ", y:" << vec.y;
}

void TestWorldCollision(const Sunset::Entity& entity, Sunset::World& world, const std::function<void(const Sunset::Entity& A, const Sunset::Entity& B)>& func)
{
	auto entitys = world.group<Sunset::TransformComponent>();
	for (const Sunset::Entity& tmp : entitys)
	{
		if (entity == tmp)
			continue;

		Sunset::TransformComponent& transA = world.get<Sunset::TransformComponent>(entity);
		Sunset::TransformComponent& transB = world.get<Sunset::TransformComponent>(tmp);
		if (Sunset::CollisionTest::Intersect(transA, transB))
		{
			func(entity, tmp);
		}
	}
}

struct ProjectilComponent : public Sunset::BaseComponent
{
	explicit ProjectilComponent(const glm::vec2& direction)
		: dir(direction)
	{ }

	bool UpdateLife(const double deltatime)
	{
		lifetime += deltatime;
		return lifetime >= 1.f;
	}

	glm::vec2 dir;
	float veloctiy = 5.f;
	float lifetime = 0.f;
};

#ifdef __EMSCRIPTEN__
void Mainloop(void* func)
{
	auto* call = static_cast<std::function<void()>*>(func);
	(*call)();
}
#endif // __EMSCRIPTEN__

void SpawnProjectil(Sunset::World& world, const glm::vec2 location, const glm::vec2& dir)
{
	Sunset::Entity projectil = world.create();
	world.emplace<Sunset::TransformComponent>(projectil, location);
	world.emplace<Sunset::RenderObjectComponent>(projectil, new Sunset::Square());
	world.emplace<ProjectilComponent>(projectil, dir);
}

int main()
{
	Sunset::Render window;

	Sunset::LayerContainer layerContainer;

	std::shared_ptr<Sunset::Layer> gameLayer = std::make_shared<Sunset::Layer>(Sunset::Layer{"Player Data"});

	layerContainer(gameLayer);

	Sunset::World world;

	Sunset::Entity player = world.create();
	world.emplace<Sunset::RenderObjectComponent>(player, new Sunset::Square());
	world.emplace<Sunset::TransformComponent>(player);

	std::shared_ptr<Sunset::Camera> cam = std::make_shared<Sunset::Camera>();

	Sunset::TransformComponent& PlayerTransComp = world.get<Sunset::TransformComponent>(player);

	glm::vec3 curseurWorldPosition = cam->GetCurseurWorldPosition();

	double Deltatime = 0.0;

	int32_t frameRate = 1;

	gameLayer->datas.emplace_back(Sunset::GameData{"PlayerLoc", Sunset::GameDataType::Vec2{}, &PlayerTransComp.location});
	gameLayer->datas.emplace_back(Sunset::GameData{"MouseLocation", Sunset::GameDataType::Vec3{}, &curseurWorldPosition});
	gameLayer->datas.emplace_back(Sunset::GameData{"Deltatime", Sunset::GameDataType::Double{}, &Deltatime});
	gameLayer->datas.emplace_back(Sunset::GameData{"Framerate", Sunset::GameDataType::Int32{}, &frameRate});

	double PreviousTime = window.GetTime();

	// Main boucle out for emscripten
	std::function<void()> func = [&]() {

		double CurrentTime = window.GetTime();
		Deltatime = CurrentTime - PreviousTime;
		PreviousTime = CurrentTime;

		frameRate = 1/Deltatime;

		constexpr float PlayerMovementSpeed = 1.f;

		curseurWorldPosition = cam->GetCurseurWorldPosition();

		if (Sunset::Inputs::IsKeyPressed(32))
		{
			glm::vec2 playerLoc = PlayerTransComp.location;
			glm::vec2 dir = glm::vec2{curseurWorldPosition.x, curseurWorldPosition.y } - playerLoc;
			dir = glm::normalize(dir);
			SpawnProjectil(world, playerLoc, dir);
		}
		if (Sunset::Inputs::IsKeyPressed(87))
		{
			PlayerTransComp.location += glm::vec2{ 0.f, PlayerMovementSpeed * Deltatime };
		}
		if (Sunset::Inputs::IsKeyPressed(83))
		{
			PlayerTransComp.location += glm::vec2{ 0.f, -PlayerMovementSpeed * Deltatime };
		}
		if (Sunset::Inputs::IsKeyPressed(65))
		{
			PlayerTransComp.location += glm::vec2{ -PlayerMovementSpeed * Deltatime, 0.f };
		}
		if (Sunset::Inputs::IsKeyPressed(68))
		{
			PlayerTransComp.location += glm::vec2{ PlayerMovementSpeed * Deltatime, 0.f };
		}

		auto Projectils = world.view<ProjectilComponent, Sunset::TransformComponent>();
		Projectils.each([&](Sunset::Entity entity, ProjectilComponent& pro, Sunset::TransformComponent& transComp){
			transComp.location += pro.dir * pro.veloctiy * (float)Deltatime;
			if (pro.UpdateLife(Deltatime))
			{
				world.destroy(entity);
			}
		});

		auto entitys = world.group<Sunset::TransformComponent>();
		for (const Sunset::Entity& entity : entitys)
		{
			TestWorldCollision(entity, world,
				[&](const Sunset::Entity& A, const Sunset::Entity& B) {

					Sunset::TransformComponent& transA = world.get<Sunset::TransformComponent>(A);
					Sunset::TransformComponent& transB = world.get<Sunset::TransformComponent>(B);

					Sunset::CollisionTest::ResolveCollision(transA, transB, Deltatime);
				});
		}

		cam->Update(Deltatime);

		// Render
		window.Begin(cam);

		layerContainer.Render();

		auto rendables = world.view<Sunset::TransformComponent, Sunset::RenderObjectComponent>();
		rendables.each([&](Sunset::TransformComponent& transform, Sunset::RenderObjectComponent& RenderObj) {
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
