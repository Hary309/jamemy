#include "Config.hpp"

#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

Config::Database Config::database;
Config::Poorchat Config::poorchat;


bool Config::load(const char* configFile)
{
	std::ifstream file(configFile);

	if (file.bad())
		return false;

	json json;
	file >> json;

	auto jsonDatabase = json["database"];
	database.host = jsonDatabase["host"];
	database.user = jsonDatabase["user"];
	database.password = jsonDatabase["password"];
	database.database = jsonDatabase["database"];

	auto jsonPoorchat = json["poorchat"];
	poorchat.host = jsonPoorchat["host"];
	poorchat.port = jsonPoorchat["port"];
	poorchat.channel = jsonPoorchat["channel"];

	return true;
}
