#pragma once

#include "Database/Database.hpp"
#include "PoorchatClient.hpp"

class App
{
private:
	Database _database;

	PoorchatClient _client;

	bool _running = false;

public:
	App();

	bool init();
	void run();
};
