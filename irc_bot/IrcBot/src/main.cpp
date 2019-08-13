#include "App.hpp"

int main()
{
	App app;

	if (app.init())
	{
		app.run();
	}
}
