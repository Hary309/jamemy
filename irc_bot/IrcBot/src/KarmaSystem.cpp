#include "KarmaSystem.hpp"

#include <algorithm>

#include <loguru.hpp>
#include <HTTPRequest.hpp>

#include "Database/Database.hpp"
#include <Config.hpp>

KarmaSystem::KarmaSystem(Database& database)
	: 
	_database(database),
	_karmaCollectorThread(&KarmaSystem::karmaCollectorThread, this)
{

}

KarmaSystem::~KarmaSystem()
{
	_running = false;

	_karmaCollectorThread.join();
}

void KarmaSystem::addLink(const std::string& authorName, const std::string& url, const std::string& message)
{
	std::optional<Author> author = { };

	std::lock_guard<std::mutex> lock(_databaseMutex);

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

	LOG_F(INFO, "Started collecting karma for %s's meme (%s)", authorName.c_str(), url.c_str());

	_karmaCollector.emplace_back<KarmaCollector>({ author.value().id, authorName, url, message, 0, std::chrono::high_resolution_clock::now() });
}

void KarmaSystem::giveKarma(const std::string& targetAuthorName, int value)
{
	std::lock_guard<std::mutex> lock(_karmaCollectorMutex);

	auto it = std::find_if(_karmaCollector.begin(), _karmaCollector.end(), [&](KarmaCollector& karmaCollector) { return karmaCollector.authorName == targetAuthorName; });

	while (it != _karmaCollector.end())
	{
		auto& karmaCollector = *it;

		karmaCollector.karma += value;

		LOG_F(INFO, "%s's meme have %d karma (%s)", targetAuthorName.c_str(), karmaCollector.karma, karmaCollector.imageUrl.c_str());

		it = std::find_if(++it, _karmaCollector.end(), [&](KarmaCollector& ka) { return ka.authorName == targetAuthorName; });
	}
}

void KarmaSystem::karmaCollectorThread()
{
	LOG_F(INFO, "Karma collector thread started!");

	while (_running)
	{
		{
			std::lock_guard<std::mutex> lock(_karmaCollectorMutex);

			auto it = std::find_if(_karmaCollector.begin(), _karmaCollector.end(), KarmaSystem::isTimedOut);

			while (it != _karmaCollector.end())
			{
				LOG_SCOPE_F(INFO, "Meme and database");

				LOG_F(INFO, "%s's meme ended up with %d karma. (url: %s)", it->authorName.c_str(), it->karma, it->imageUrl.c_str());

				if (it->karma > 0)
				{
					LOG_F(INFO, "Adding to database");

					std::lock_guard<std::mutex> lock(_databaseMutex);
					auto memeId = _database.addMeme(it->authorId, it->imageUrl, it->message);

					if (memeId == 0)
					{
						LOG_F(WARNING, "Cannot add meme :(");
					}
					else
					{
						_database.setKarma(memeId, it->karma);

						http::Request request(Config::apiAddress + "/update/" + std::to_string(memeId));
						request.send();
					}
				}
				else
				{
					LOG_F(INFO, "Meme won't be added");
				}

				it = _karmaCollector.erase(it);

				it = std::find_if(it, _karmaCollector.end(), KarmaSystem::isTimedOut);
			}
		}

		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
}

bool KarmaSystem::isTimedOut(KarmaCollector& karmaCollector)
{
	std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - karmaCollector.startTime;

	return duration.count() > CollectionTime;
}
