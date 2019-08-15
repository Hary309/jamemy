#pragma once

#include <string>
#include <vector>
#include <chrono>

#include <atomic>
#include <thread>
#include <mutex>

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

	static constexpr float MaxTimeout = 60.f;

private:
	Database& _database;

	KarmaActivites_t _karmaActive;

	std::mutex _karmaActiveMutex;
	std::thread _karmaActiveThread;

	std::atomic<bool> _running{ true };

public:
	KarmaSystem(Database& database);
	~KarmaSystem();

	void addLink(const std::string& author, const std::string& url);

	void giveKarma(const std::string& target, int value);

private:
	void karmaActiveThread();

	static bool isKarmaTimedout(KarmaActive& karmaActive);
};
