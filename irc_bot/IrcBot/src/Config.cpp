#include "Config.hpp"

#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

Config::Database Config::database;
Config::Poorchat Config::poorchat;
Config::Email Config::email;

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

	auto jsonEmail = json["email"];
	email.host = jsonEmail["host"];
	email.port = jsonEmail["port"];
	email.user = jsonEmail["user"];
	email.password = jsonEmail["password"];
	email.mailTo = jsonEmail["mailTo"];

	return true;
}
