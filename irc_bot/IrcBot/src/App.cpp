#include "App.hpp"

#include <iostream>
#include <thread>
#include <chrono>

#include <loguru.hpp>

#include "Config.hpp"

App::App() : _karmaSystem(_database), _client(_karmaSystem)
{
}

bool App::init()
{
	LOG_F(INFO, "Loading config from file config.json...");

	if (!Config::load("config.json"))
	{
		LOG_F(ERROR, "Cannot load config!");
		return false;
	}

	const auto& dbCfg = Config::database;

	LOG_F(INFO, "Connecting to database Host: '%s', User: '%s', Password: '%s', Database: '%s'...", 
		dbCfg.host.c_str(), 
		dbCfg.user.c_str(), 
		dbCfg.password.c_str(), 
		dbCfg.database.c_str());

	if (!_database.connect(dbCfg.host.c_str(), dbCfg.user.c_str(), dbCfg.password.c_str(), dbCfg.database.c_str()))
	{
		LOG_F(ERROR, "Cannot connet to database!");
		return false;
	}

	const auto& pcCfg = Config::poorchat;

	LOG_F(INFO, "Connecting to IRC %s:%d %s...", 
		pcCfg.host.c_str(), 
		pcCfg.port, 
		pcCfg.channel.c_str());

	if (!_client.connect(pcCfg.host.c_str(), pcCfg.port, pcCfg.channel.c_str()))
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
