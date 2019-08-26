#pragma once

#include <string>
#include <vector>
#include <chrono>

#include <atomic>
#include <thread>
#include <mutex>

#include "Database/Types.hpp"

class Database;

struct KarmaCollector
{
	Id authorId;
	std::string authorName;
	std::string imageUrl;
	std::string message;
	int karma;
	std::chrono::time_point<std::chrono::steady_clock> startTime;
};

class KarmaSystem
{
	using KarmaCollectors_t = std::vector<KarmaCollector>;

	static constexpr float CollectionTime = 70.f;

private:
	Database& _database;
	std::mutex _databaseMutex;

	KarmaCollectors_t _karmaCollector;

	std::mutex _karmaCollectorMutex;
	std::thread _karmaCollectorThread;

	std::atomic<bool> _running{ true };

public:
	KarmaSystem(Database& database);
	~KarmaSystem();

	void addLink(const std::string& author, const std::string& url, const std::string& message);

	void giveKarma(const std::string& target, int value);

private:
	void karmaCollectorThread();

	static bool isTimedOut(KarmaCollector& karmaCollector);
};
