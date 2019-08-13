#pragma once

#include "Database/Database.hpp"

class App
{
private:
	Database _database;

	bool _running = false;

public:
	bool init();
	void run();
};
