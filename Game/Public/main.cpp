
#include "Camera.h"
#include "Inputs.h"
#include "Object.h"
#include "Render.h"
#include "RenderObject.h"

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#endif // __EMSCRIPTEN__


#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <random>

void CreateGround(Sunset::World& world)
{
	Sunset::Entity Ground = world.create();
	world.emplace<Sunset::RenderObjectComponent>(Ground, new Sunset::Square());
	Sunset::TransformComponent& TransComp = world.emplace<Sunset::TransformComponent>(Ground, glm::vec2{ 0, -2 });
	TransComp.size = glm::vec2{ 5, 5 };
	TransComp.bStatic = true;
}

glm::vec2 minBounds{ -1.f, -1.f };
glm::vec2 maxBounds{ +1.f, +1.f };

constexpr float minX = -1.f, maxX = 1.f;
constexpr float minY = -1.f, maxY = 1.f;

static std::random_device rd;
static std::mt19937       gen(rd());
static std::uniform_real_distribution<float> distX(minX, maxX);
static std::uniform_real_distribution<float> distY(minY, maxY);

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

	Sunset::World world;

	std::shared_ptr<Sunset::Camera> cam = std::make_shared<Sunset::Camera>();

	for (int i = 0; i < 20; ++i)
	{
		glm::vec2 randPos{ distX(gen), distY(gen) };

		Sunset::Entity boid = world.create();
		world.emplace<Sunset::TransformComponent>(boid, randPos);
		world.emplace<Sunset::RenderObjectComponent>(boid, new Sunset::Square());
		world.emplace<Sunset::PhysicComponent>(boid);
	}

	double PreviousTime = window.GetTime();

	// Main boucle out for emscripten
	std::function<void()> func = [&]() {
		double CurrentTime = window.GetTime();
		double Deltatime = CurrentTime - PreviousTime;
		PreviousTime = CurrentTime;

		if (Sunset::Inputs::IsKeyPressed(256))
		{
			window.Close(true);
		}

		//if (Sunset::Inputs::IsMouseButton(Sunset::Inputs::Pressed{}, 0))
		//{
		//	glm::vec3 loc = cam->GetCurseurWorldPosition();

		//	Sunset::Entity entity = world.create();
		//	world.emplace<Sunset::TransformComponent>(entity, glm::vec2{loc.x, loc.y});
		//	world.emplace<Sunset::RenderObjectComponent>(entity, new Sunset::Square());
		//	world.emplace<Sunset::PhysicComponent>(entity);
		//}

		if (Sunset::Inputs::IsKeyPressed(32))
		{
			world.clear();
			CreateGround(world);
			std::cerr << "Clear\n";
		}

		static std::mt19937                              rng{ std::random_device{}() };
		static std::uniform_real_distribution<float>     angleDist(0.0f, glm::two_pi<float>());

		world.view<Sunset::PhysicComponent>().each(
			[&](auto entity, Sunset::PhysicComponent& phys) {
				if (phys.velocity == glm::vec2{ 0.0f }) {
					float angle = angleDist(rng);
					phys.velocity = glm::vec2{ std::cos(angle), std::sin(angle) } *phys.speed;
				}
			});

		world.view<Sunset::TransformComponent>().each(
			[&](auto entity, Sunset::TransformComponent& trans) {
				TestWorldCollision(entity, world,
					[&](const Sunset::Entity& A, const Sunset::Entity& B) {
						auto* physA = world.try_get<Sunset::PhysicComponent>(A);
						auto* physB = world.try_get<Sunset::PhysicComponent>(B);
						if (!physA || !physB) return;

						physA->velocity = glm::normalize(
							physA->velocity + physB->velocity
						) * physA->speed;
					}
				);
			});

		world.view<Sunset::TransformComponent, Sunset::PhysicComponent>().each(
			[&](auto entity,
				Sunset::TransformComponent& trans,
				Sunset::PhysicComponent& phys)
			{
				trans.location += phys.velocity * static_cast<float>(Deltatime);

				if (trans.location.x < minBounds.x) {
					trans.location.x = minBounds.x;
					phys.velocity.x *= -1;
				}
				else if (trans.location.x > maxBounds.x) {
					trans.location.x = maxBounds.x;
					phys.velocity.x *= -1;
				}

				if (trans.location.y < minBounds.y) {
					trans.location.y = minBounds.y;
					phys.velocity.y *= -1;
				}
				else if (trans.location.y > maxBounds.y) {
					trans.location.y = maxBounds.y;
					phys.velocity.y *= -1;
				}
			});

		//auto PhysEntityList = world.view<Sunset::TransformComponent, Sunset::PhysicComponent>();
		//PhysEntityList.each([&](Sunset::TransformComponent& transform, Sunset::PhysicComponent& physComp){
		//	physComp.ApplyPhysic(Deltatime, transform);
		//});

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
