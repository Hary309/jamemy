#pragma once

#include <re2/re2.h>

#include "Socket.hpp"
#include "Database/Database.hpp"

class PoorchatClient
{
private:
	Socket _client;
	Database& _database;

	int _stage = 0;

	bool _joinedChannel = false;

	re2::RE2 _privMsg_re{ ":(.*)!.* PRIVMSG .* :(.*)\r\n" };
	re2::RE2 _url_re{ "\\b(([\\w-]+:\\/\\/?|www[.])[^\\s()<>]+(?:\\([\\w\\d]+\\)|([^[:punct:]\\s]|\\/)))" };

public:
	PoorchatClient(Database& database);

	bool connect();

	void update();

private:
	void parse(const std::string& message);
};
