#pragma once

#include <regex>

#include "Socket.hpp"
#include "Database/Database.hpp"

class PoorchatClient
{
private:
	Socket _client;
	Database& _database;

	int _stage = 0;

	bool _joinedChannel = false;

	std::regex _privMsg_re {":(.*)!.* PRIVMSG (.*) :(.*)\r\n"};

public:
	PoorchatClient(Database& database);

	bool connect();

	void update();

private:
	void parse(const std::string& message);
};
