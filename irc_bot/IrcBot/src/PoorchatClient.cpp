#include "PoorchatClient.hpp"

#include <string>
#include <algorithm>
#include <regex>

// undefine windows' min function
#undef min

#include <re2/stringpiece.h>
#include <re2/re2.h>

#include <loguru.hpp>

#include "KarmaSystem.hpp"

PoorchatClient::PoorchatClient(KarmaSystem& karmaSystem)
	: _karmaSystem(karmaSystem)
{
	_client.init();
}

bool PoorchatClient::connect()
{
	if (_client.connect("irc.poorchat.net", 6667))
	{
		_client.send("USER IrcBotUser 8 * :IRC Client\r\n");

		return true;
	}

	return false;
}

void PoorchatClient::update()
{
	if (_client.isConnected())
	{
		auto data = _client.receive();
		LOG_F(9, "Chat: %s", data.c_str());


		if (!_joinedChannel)
		{
			_client.send("JOIN #jaharry\r\n");
			_joinedChannel = true;
		}

		if (data.length() >= 4 && data[0] == 'P' && data[1] == 'I' && data[2] == 'N' && data[3] == 'G')
		{
			auto pong = "PONG " + data.substr(5);

			LOG_F(9, "Chat: %s", pong.c_str());

			_client.send(pong.c_str());

			return;
		}

		std::string name, message;

		if (re2::RE2::FullMatch(data, _privMsg_re, &name, &message))
		{
			// find url
			{
				std::string url;

				if (re2::RE2::PartialMatch(message, _url_re, &url))
				{
					_karmaSystem.addLink(name, url);
				}
			}

			// find plus karma
			findKarmaAction(message, name, _karmaPlus_re, 1);
			findKarmaAction(message, name, _karmaMinus_re, -1);
		}
	}
}

void PoorchatClient::findKarmaAction(const std::string& message, const std::string& messageAuthor, const re2::RE2& regex, int value)
{
	std::string targetAuthorName;

	if (re2::RE2::PartialMatch(message, regex, &targetAuthorName))
	{
		//if (messageAuthor != targetAuthorName)
		{
			LOG_SCOPE_F(INFO, "Karma");
			LOG_F(INFO, "%s gave %d karma to %s!", messageAuthor.c_str(), value, targetAuthorName.c_str());
			_karmaSystem.giveKarma(targetAuthorName, value);
		}
	}
}
