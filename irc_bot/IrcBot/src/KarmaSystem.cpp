#include "KarmaSystem.hpp"

#include <algorithm>

#include <loguru.hpp>

#include "Database/Database.hpp"

KarmaSystem::KarmaSystem(Database& database)
	: 
	_database(database),
	_karmaActiveThread(&KarmaSystem::karmaActiveThread, this)
{

}

KarmaSystem::~KarmaSystem()
{
	_running = false;

	_karmaActiveThread.join();
}

void KarmaSystem::addLink(const std::string& authorName, const std::string& url)
{
	std::optional<Author> author = { };

	author = _database.getAuthor(authorName);

	if (!author.has_value())
	{
		auto id = _database.addAuthor(authorName);

		author = _database.getAuthor(id);

		if (!author.has_value())
		{
			LOG_F(WARNING, "Cannot find and add author with name %s :(", authorName.c_str());

			return;
		}


		LOG_F(INFO, "Added author %s", authorName.c_str());
	}

	auto memeId = _database.addMeme(author.value().id, url);

	if (memeId == 0)
	{
		LOG_F(WARNING, "Cannot add meme :( (%s)", _database.getError());
		return;
	}

	LOG_F(INFO, "Added meme: %s by %s", url.c_str(), authorName.c_str());

	_karmaActive.emplace_back<KarmaActive>({author.value().id, authorName, memeId, 0, std::chrono::high_resolution_clock::now() });
}

void KarmaSystem::giveKarma(const std::string& targetAuthorName, int value)
{
	std::lock_guard<std::mutex> lock(_karmaActiveMutex);

	auto it = std::find_if(_karmaActive.begin(), _karmaActive.end(), [&](KarmaActive& karmaActive) { return karmaActive.authorName == targetAuthorName; });

	while (it != _karmaActive.end())
	{
		auto& karmaActive = *it;

		karmaActive.karma += value;

		LOG_F(INFO, "%s's meme (%lld) have %d karma", targetAuthorName.c_str(), karmaActive.memeId, karmaActive.karma);

		_database.setKarma(karmaActive.memeId, karmaActive.karma);

		it = std::find_if(++it, _karmaActive.end(), [&](KarmaActive& ka) { return ka.authorName == targetAuthorName; });
	}
}

void KarmaSystem::karmaActiveThread()
{
	LOG_F(INFO, "Karma active thread started!");

	while (_running)
	{
		{
			std::lock_guard<std::mutex> lock(_karmaActiveMutex);

			auto it = std::find_if(_karmaActive.begin(), _karmaActive.end(), KarmaSystem::isKarmaTimedout);

			while (it != _karmaActive.end())
			{
				LOG_F(INFO, "Removing active karma with id %d by %s", it->memeId, it->authorName.c_str());

				it = _karmaActive.erase(it);

				it = std::find_if(it, _karmaActive.end(), KarmaSystem::isKarmaTimedout);
			}

		}

		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
}

bool KarmaSystem::isKarmaTimedout(KarmaActive& karmaActive)
{
	std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - karmaActive.startTime;

	return duration.count() > MaxTimeout;
}
