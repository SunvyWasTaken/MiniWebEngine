// Sunset inc.

#include "Application.h"

int main()
{
	Sunset::Engine::Create<Sunset::Engine>()->Run();
	Sunset::Engine::Destroy();
}