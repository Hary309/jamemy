#include "KarmaSystem.hpp"

#include <algorithm>

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
			printf("Cannot find and add author :(\n");
			return;
		}

		printf("Added author %s\n", author.value().name.c_str());
	}

	auto memeId = _database.addMeme(author.value().id, url);

	if (memeId == 0)
	{
		printf("Cannot add meme :( (%s)\n", _database.getError());
		return;
	}

	printf("Added meme: %s\n", url.c_str());

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

		printf("Time left: %f\n", 60.f - duration.count());

		if (duration.count() > 60.f)
		{
			toDelete.push_back(it);
			continue;
		}

		karmaActive.karma += value;

		printf("%s's meme (%lld) have %d karma\n", targetAuthorName.c_str(), karmaActive.memeId, karmaActive.karma);

		_database.setKarma(karmaActive.memeId, karmaActive.karma);
	}

	for (auto& it : toDelete)
	{
		_karmaActive.erase(it);
	}
}
