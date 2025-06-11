// Sunset inc.

#ifdef EDITOR
#include "EditorApplication.h"
#else
#include "Application.h"
#endif // EDITOR

int main()
{
#ifdef EDITOR
	Sunset::Engine::Create<Sunset::EditorEngine>()->Run();
	Sunset::Engine::Destroy();
#else
	Sunset::Engine::Create<Sunset::Engine>()->Run();
	Sunset::Engine::Destroy();
#endif
}