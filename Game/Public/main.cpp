
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

std::ostream& operator<<(std::ostream& io, const glm::vec2& vec)
{
	return io << "x:" << vec.x << ", y:" << vec.y;
}

void TestWorldCollision(const Sunset::Entity& entity, Sunset::World& world, const std::function<void(const Sunset::Entity& A, const Sunset::Entity& B)>& func)
{
	auto entitys = world.view<Sunset::TransformComponent>();
	entitys.each([&](Sunset::Entity tmp, Sunset::TransformComponent& transComp)
		{
			if (entity == tmp)
				return;

			if (Sunset::CollisionTest::Intersect(entity, tmp, world))
			{
				func(entity, tmp);
			}
		});
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

struct UpdateComponent : public Sunset::BaseComponent
{
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
	world.emplace<Sunset::TransformComponent>(projectil, location + dir * 0.1f);
	world.emplace<Sunset::RenderObjectComponent>(projectil, new Sunset::Square({1, 3}));
	world.emplace<ProjectilComponent>(projectil, dir);
	world.emplace<Sunset::CollisionComponent>(projectil);
}

void SpawnEnnemie(Sunset::World& world, std::vector<Sunset::Entity>& entityToDestroy)
{
	Sunset::Entity ennemy = world.create();
	// Get a random location btw -1 1
	world.emplace<Sunset::TransformComponent>(ennemy);
	world.emplace<Sunset::RenderObjectComponent>(ennemy, new Sunset::Square({2, 3}));
	Sunset::CollisionComponent& colliComp = world.emplace<Sunset::CollisionComponent>(ennemy);
	colliComp.OnCollision = [&, ennemy](){
		entityToDestroy.emplace_back(ennemy);
	};
	world.emplace<UpdateComponent>(ennemy);
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
	std::vector<Sunset::Entity> EntityToDestroy;

	Sunset::Entity player = world.create();
	world.emplace<Sunset::RenderObjectComponent>(player, new Sunset::Square({0, 3}));
	world.emplace<Sunset::TransformComponent>(player);

	std::shared_ptr<Sunset::Camera> cam = std::make_shared<Sunset::Camera>();

	Sunset::TransformComponent& PlayerTransComp = world.get<Sunset::TransformComponent>(player);

	glm::vec3 curseurWorldPosition = cam->GetCurseurWorldPosition();

	std::array<bool, 5> ListKeyPressed;
	ListKeyPressed.fill(false);

	double Deltatime = 0.0;

	int32_t frameRate = 1;

	float CurrSpawnTimeEnnemy = 0;

#ifndef __EMSCRIPTEN__
	gameLayer->datas.emplace_back(Sunset::GameData{"PlayerLoc", Sunset::GameDataType::Vec2{}, &PlayerTransComp.location});
	gameLayer->datas.emplace_back(Sunset::GameData{"MouseLocation", Sunset::GameDataType::Vec3{}, &curseurWorldPosition});
	gameLayer->datas.emplace_back(Sunset::GameData{"Deltatime", Sunset::GameDataType::Double{}, &Deltatime});
	gameLayer->datas.emplace_back(Sunset::GameData{"Framerate", Sunset::GameDataType::Int32{}, &frameRate});
#endif // !__EMSCRIPTEN__

	double PreviousTime = window.GetTime();

	// Main boucle out for emscripten
	std::function<void()> func = [&]() {

		double CurrentTime = window.GetTime();
		Deltatime = CurrentTime - PreviousTime;
		PreviousTime = CurrentTime;

		frameRate = 1/Deltatime;

		constexpr float PlayerMovementSpeed = 1.f;

		curseurWorldPosition = cam->GetCurseurWorldPosition();

		CurrSpawnTimeEnnemy += Deltatime;
		if (CurrSpawnTimeEnnemy >= 5.f)
		{
			CurrSpawnTimeEnnemy = 0;
			SpawnEnnemie(world, EntityToDestroy);
		}

		if (Sunset::Inputs::IsMouseButtonPressed(0) && !ListKeyPressed[0])
		{
			glm::vec2 playerLoc = PlayerTransComp.location;
			glm::vec2 dir = glm::vec2{curseurWorldPosition.x, curseurWorldPosition.y } - playerLoc;
			dir = glm::normalize(dir);
			SpawnProjectil(world, playerLoc, dir);
			ListKeyPressed[0] = true;
		}
		if (Sunset::Inputs::IsMouseButtonReleased(0))
		{
			ListKeyPressed[0] = false;
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

		auto entitys = world.view<Sunset::TransformComponent>();
		entitys.each([&](Sunset::Entity entity, Sunset::TransformComponent& transComp)
		{
			TestWorldCollision(entity, world,
				[&](const Sunset::Entity& A, const Sunset::Entity& B) {
					auto [transA, colliA] = world.get<Sunset::TransformComponent, Sunset::CollisionComponent>(A);
					auto [transB, colliB] = world.get<Sunset::TransformComponent, Sunset::CollisionComponent>(B);

					Sunset::CollisionTest::ResolveCollision(transA, transB, Deltatime);

					world.destroy(entity);
					world.destroy(B);
				});
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
