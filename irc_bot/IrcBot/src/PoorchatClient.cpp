#include "PoorchatClient.hpp"

#include <string>
#include <algorithm>
#include <regex>

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

		std::smatch match;

		if (std::regex_match(data, match, _privMsg_re))
		{
			if (match.size() == 4)
			{
				auto name = match[1].str();
				auto channel = match[2].str();
				auto message = match[3].str();

				printf("Extra message: [%s] %s: %s\n", channel.c_str(), name.c_str(), message.c_str());
			}
			else
			{
				printf("Nope\n");
			}
		}
	}
}
