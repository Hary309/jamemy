#include "App.hpp"

#include "loguru.hpp"

int main()
{
	loguru::g_preamble_thread = false;
	loguru::g_preamble_uptime = false;
	loguru::add_file("chat_and_logs.log", loguru::Append, loguru::Verbosity_MAX);
	loguru::add_file("logs.log", loguru::Append, loguru::Verbosity_8);

#ifdef WIN32
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		LOG_F(ERROR, "Unable to initialize Winsock.");
		return -1;
	}
#endif

	App app;

	if (app.init())
	{
		app.run();
	}

#ifdef WIN32
	WSACleanup();
#endif

	LOG_F(INFO, "Application is closing...");

	std::this_thread::sleep_for(std::chrono::seconds(15));
}
