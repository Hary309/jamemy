#include "App.hpp"

#include <iostream>

bool App::init()
{
	if (!_database.connect("127.0.0.1", "root", "", "jamemy"))
	{
		return false;
	}

	_running = true;

	return true;
}

void App::run()
{
	std::string command;

	//while (_running)
	{
		do
		{
			std::cout << ">";
			std::cin >> command;

			if (command == "add")
			{
				std::cout << "What do you want to add? (author, meme)\n";

				std::cout << ">";
				std::cin >> command;

				if (command == "author")
				{
					std::cout << "Type username: ";
					std::cin >> command;

					if (!_database.addAuthor(command))
					{
						std::cout << "Error" << _database.getError() << std::endl;
					}
				}
				else if (command == "meme")
				{
					Id id;
					std::string url;

					std::cout << "Type user's id: ";
					std::cin >> id;

					std::cout << "Type url: ";
					std::cin >> url;

					auto memeId = _database.addMeme(id, url);

					if (memeId == 0)
					{
						std::cout << "Error" << _database.getError() << std::endl;
					}
				}
			}
			else if (command == "list")
			{
				std::cout << "What do you want to list? (author, meme)\n";

				std::cout << ">";
				std::cin >> command;

				printf("Listing %s\n", command.c_str());

				mysql_query(&_database.getMySQL(), ("SELECT * FROM " + command + ";").c_str());

				auto res = mysql_store_result(&_database.getMySQL());

				MYSQL_ROW row;

				while ((row = mysql_fetch_row(res)) != 0)
				{
					for (uint32_t i = 0; i < mysql_num_fields(res); i++)
						std::cout << row[i] << " ";
					std::cout << "\n";
				}

			}
			else if (command == "givekarma")
			{
				Id id;
				int karmaValue;

				std::cout << "Meme id: ";
				std::cin >> id;

				std::cout << "Karma value: ";
				std::cin >> karmaValue;

				if (!_database.giveKarma(id, karmaValue))
				{
					std::cout << "Error" << _database.getError() << std::endl;
				}
			}

		} while (command != "exit");
	}
}
