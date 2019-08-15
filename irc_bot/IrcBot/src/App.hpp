#pragma once

#include "Database/Database.hpp"
#include "KarmaSystem.hpp"
#include "PoorchatClient.hpp"

class App
{
private:
	Database _database;
	KarmaSystem _karmaSystem;

	PoorchatClient _client;

	bool _running = false;

public:
	App();

	bool init();
	void run();
};
