#pragma once

#include <string>
#include <cstdlib>

class Config
{
public:
	struct Database
	{
		std::string host;
		std::string user;
		std::string password;
		std::string database;
	};

	struct Poorchat
	{
		std::string host;
		uint16_t port;
		std::string channel;
	};

public:
	static Database database;
	static Poorchat poorchat;

public:

	static bool load(const char* configFile);
};
