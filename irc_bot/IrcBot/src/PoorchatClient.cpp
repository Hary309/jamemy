#include "PoorchatClient.hpp"

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

		std::cout << data << "\n";

		if (data.length() >= 4 && data[0] == 'P' && data[1] == 'I' && data[2] == 'N' && data[3] == 'G')
		{
			auto pong = "PONG " + data.substr(5);

			std::cout << "" << pong << "\n";

			_client.send(pong.c_str());
		}


		if (!_joinedChannel)
		{
			_client.send("JOIN #jadisco\r\n");
			_joinedChannel = true;
		}
	}
}
