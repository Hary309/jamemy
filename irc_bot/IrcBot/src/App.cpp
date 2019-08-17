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
	LOG_F(INFO, "Loading config from file config.json...");

	if (!Config::load("config.json"))
	{
		LOG_F(ERROR, "Cannot load config!");
		return false;
	}

	if (!initDatabase(Config::database))
	{
		LOG_F(ERROR, "Cannot connet to database!");
		return false;
	}

	if (!initPoorchatClient(Config::poorchat))
	{
		return false;
	}

	if (!initSMTPClient(Config::email))
	{
		LOG_F(ERROR, "Cannot init SMTP Client!");
	}

	LOG_F(INFO, "App started!");

	_running = true;

	return true;
}

void App::run()
{
	_client.run();

	if (_email.isReady())
	{
		_email.sendFault();
	}
}

bool App::initDatabase(Config::Database& cfg)
{
	LOG_F(INFO, "Connecting to database Host: '%s', User: '%s', Password: '%s', Database: '%s'...",
		cfg.host.c_str(),
		cfg.user.c_str(),
		cfg.password.c_str(),
		cfg.database.c_str());

	return _database.connect(cfg.host.c_str(), cfg.user.c_str(), cfg.password.c_str(), cfg.database.c_str());
}

bool App::initPoorchatClient(Config::Poorchat& cfg)
{
	LOG_F(INFO, "Connecting to IRC %s:%d %s...",
		cfg.host.c_str(),
		cfg.port,
		cfg.channel.c_str());

	if (!_client.init())
	{
		LOG_F(ERROR, "Cannot init IRC Client!");
		return false;
	}

	if (!_client.connect(cfg.host.c_str(), cfg.port, cfg.channel.c_str()))
	{
		LOG_F(ERROR, "Cannot connet to IRC! (%s)", _client.getError());
		return false;
	}

	return true;
}

bool App::initSMTPClient(Config::Email& cfg)
{
	LOG_F(INFO, "Initializing SMTP Client %s:%s User: '%s' Password: '%s' mailTo: '%s'...",
		cfg.host.c_str(),
		cfg.port.c_str(),
		cfg.user.c_str(),
		cfg.password.c_str(),
		cfg.mailTo.c_str());


	return _email.init(cfg.host.c_str(), cfg.port.c_str(), cfg.user.c_str(), cfg.password.c_str(), cfg.mailTo.c_str());
}
