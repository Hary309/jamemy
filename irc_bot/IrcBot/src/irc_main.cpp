#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <iostream>


#include "Socket.hpp"


int main()
{
	Socket socket;
	socket.init();
	socket.connect("irc.poorchat.net", 6667);

	bool time = 0;

	for (;;)
	{
		if (socket.isConnected())
		{
			if (time == 0)
			{
				time++;
				socket.send("NICK TestowyBotNick\r\n");
				socket.send("USER TestowyBotUser 8 * :Cpp IRC Client\r\n");
			}
			else if (time == 1)
			{
				socket.send("JOIN #jadisco\r\n");
			}

			auto data = socket.receive();

			std::cout << data << "\n";

		}
	}
}
