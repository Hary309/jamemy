#pragma once

#include "Database/Database.hpp"
#include "KarmaSystem.hpp"
#include "IRC/PoorchatClientV2.hpp"

class App
{
private:
	Database _database;
	KarmaSystem _karmaSystem;

	PoorchatClientV2 _client;

	bool _running = false;

public:
	App();

	bool init();
	void run();
};
