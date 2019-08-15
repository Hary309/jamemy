#include "App.hpp"

#include "loguru.hpp"

int main()
{
	loguru::g_preamble_thread = false;
	loguru::g_preamble_uptime = false;
	loguru::add_file("chat_and_logs.log", loguru::Append, loguru::Verbosity_MAX);
	loguru::add_file("logs.log", loguru::Append, loguru::Verbosity_8);

	App app;

	if (app.init())
	{
		app.run();
	}
}
