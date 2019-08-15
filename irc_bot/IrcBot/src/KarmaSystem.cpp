#include "KarmaSystem.hpp"

#include <algorithm>

#include <loguru.hpp>

#include "Database/Database.hpp"

KarmaSystem::KarmaSystem(Database& database)
	: _database(database)
{
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

	LOG_F(INFO, "Added meme: %s", url.c_str());

	_karmaActive.emplace_back<KarmaActive>({author.value().id, authorName, memeId, 0, std::chrono::high_resolution_clock::now() });
}

void KarmaSystem::giveKarma(const std::string& targetAuthorName, int value)
{
	std::vector<KarmaActivites_t::iterator> toDelete;

	auto it = std::find_if(_karmaActive.begin(), _karmaActive.end(), [&](KarmaActive& karmaActive) { return karmaActive.authorName == targetAuthorName; });

	for (; it != _karmaActive.end(); it++)
	{
		auto& karmaActive = *it;

		std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - karmaActive.startTime;

		LOG_F(INFO, "Time left: %f", 60.0 - duration.count());

		if (duration.count() > 60.f)
		{
			toDelete.push_back(it);
			continue;
		}

		karmaActive.karma += value;

		LOG_F(INFO, "%s's meme (%lld) have %d karma", targetAuthorName.c_str(), karmaActive.memeId, karmaActive.karma);

		_database.setKarma(karmaActive.memeId, karmaActive.karma);
	}

	for (auto& it : toDelete)
	{
		_karmaActive.erase(it);
	}
}
