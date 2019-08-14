#pragma once

#include "Socket.hpp"
#include "Database/Database.hpp"

class PoorchatClient
{
private:
	Socket _client;
	Database& _database;

	int _stage = 0;

	bool _joinedChannel = false;

public:
	PoorchatClient(Database& database);

	bool connect();

	void update();
};
