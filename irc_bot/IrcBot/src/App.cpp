#include "App.hpp"

#include <iostream>
#include <thread>
#include <chrono>

App::App() : _karmaSystem(_database), _client(_karmaSystem)
{
}

bool App::init()
{
	if (!_database.connect("127.0.0.1", "root", "", "jamemy"))
	{
		return false;
	}

	if (!_client.connect())
	{
		return false;
	}

	_running = true;

	return true;
}

void App::run()
{
	using namespace std::literals::chrono_literals;

	std::string command;

	while (_running)
	{
		_client.update();

		std::this_thread::sleep_for(10ms);
	}
}
