#pragma once

#include "Config.hpp"
#include "Database/Database.hpp"
#include "KarmaSystem.hpp"
#include "IRC/PoorchatClientV2.hpp"
#include "SMTPClient.hpp"

class App
{
private:
	Database _database;
	KarmaSystem _karmaSystem;

	PoorchatClientV2 _client;

	SMTPClient _email;

	bool _running = false;

public:
	App();

	bool init();
	void run();

private:
	bool initDatabase(Config::Database& dbCfg);
	bool initPoorchatClient(Config::Poorchat& pcCfg);
	bool initSMTPClient(Config::Mail& emailCfg);
};
