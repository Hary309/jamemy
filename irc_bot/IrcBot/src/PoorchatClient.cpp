
#include "PoorchatClient.hpp"

#include <string>
#include <algorithm>
#include <regex>

// undefine windows' min function
#undef min

#include <re2/stringpiece.h>
#include <re2/re2.h>


PoorchatClient::PoorchatClient(Database& database)
	: _database(database)
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
		std::cout << data;

		if (!_joinedChannel)
		{
			_client.send("JOIN #jaharry\r\n");
			_joinedChannel = true;
		}

		if (data.length() >= 4 && data[0] == 'P' && data[1] == 'I' && data[2] == 'N' && data[3] == 'G')
		{
			auto pong = "PONG " + data.substr(5);

			std::cout << "" << pong;

			_client.send(pong.c_str());

			return;
		}

		std::string name, message;

		if (re2::RE2::FullMatch(data, _privMsg_re, &name, &message))
		{
			std::string url;

			// find url
			if (re2::RE2::PartialMatch(message, _url_re, &url))
			{
				printf("URL: %s\n", url.c_str());
			}
		}
		else
		{
			printf("nope\n");
		}
	}
}
