#include "App.hpp"

#include <iostream>
#include <thread>
#include <chrono>

#include <loguru.hpp>

App::App() : _karmaSystem(_database), _client(_karmaSystem)
{
}

bool App::init()
{
	LOG_F(INFO, "Connecting to database...");

	if (!_database.connect("127.0.0.1", "root", "", "jamemy"))
	{
		LOG_F(ERROR, "Cannot connet to database!");
		return false;
	}

	LOG_F(INFO, "Connecting to IRC...");

	if (!_client.connect())
	{
		LOG_F(ERROR, "Cannot connet to IRC!");
		return false;
	}

	_running = true;

	return true;
}

void App::run()
{
	using namespace std::literals::chrono_literals;

	std::string command;

	LOG_F(INFO, "Application started!");
	while (_running)
	{
		_client.update();

		std::this_thread::sleep_for(10ms);
	}
}
