#pragma once

#include <string>
#include <vector>
#include <chrono>

#include "Database/Types.hpp"

class Database;

struct KarmaActive
{
	Id authorId;
	std::string authorName;
	Id memeId;
	int karma;
	std::chrono::time_point<std::chrono::steady_clock> startTime;
};

class KarmaSystem
{
	using KarmaActivites_t = std::vector<KarmaActive>;

private:
	Database& _database;

	KarmaActivites_t _karmaActive;

public:
	KarmaSystem(Database& database);

	void addLink(const std::string& author, const std::string& url);

	void giveKarma(const std::string& target, int value);
};
