// Sunset inc.

#include "Application.h"

int main()
{
	std::unique_ptr<Sunset::Engine> app = std::make_unique<Sunset::Engine>();
	app->Run();
}