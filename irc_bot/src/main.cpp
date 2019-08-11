#include <iostream>
#include <string>
#include <mysql.h>

void listTable(MYSQL& mysql, const std::string& tableName)
{
    printf("Listing %s\n", tableName.c_str());

    mysql_query(&mysql, ("SELECT * FROM " + tableName + ";").c_str());

    auto res = mysql_store_result(&mysql);

    MYSQL_ROW row;

    while((row = mysql_fetch_row(res)) != 0)
    {
        for(uint32_t i = 0; i < mysql_num_fields(res); i++)
            std::cout << row[i] << " ";
        std::cout << "\n";
    }

}

int main()
{
    MYSQL mysql;
    mysql_init(&mysql);

    if (mysql_real_connect(&mysql, "127.0.0.1", "root", "", "jamemy", 0, nullptr, 0))
    // if (mysql_real_connect(&mysql, "mysql.ct8.pl", "m5189_jamemy", "WwGE5AgESgYwgmr", "m5189_jamemy", 0, nullptr, 0))
    {
        printf("Connected!\n");
    }
    else 
    {
        printf("Cannot connect! %s\n", mysql_error(&mysql));
        return -1;
    }

    std::string command;

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

                if (mysql_query(&mysql, ("INSERT INTO author VALUES(NULL, '" + command + "');").c_str()) != 0)
                {
                    std::cout << "Error" << mysql_error(&mysql) << std::endl;
                }
            }
            else if (command == "meme")
            {
                std::string id, url;

                std::cout << "Type user's id: ";
                std::cin >> id;

                std::cout << "Type url: ";
                std::cin >> url;

                if (mysql_query(&mysql, ("INSERT INTO meme VALUES(NULL, " + id + ", '" + url + "');").c_str()) != 0)
                {
                    std::cout << "Error" << mysql_error(&mysql) << std::endl;
                }
            }
        }
        else if (command == "list")
        {
            std::cout << "What do you want to list? (author, meme)\n";

            std::cout << ">";
            std::cin >> command;

            listTable(mysql, command);
        }

    } while (command != "exit");

    mysql_close(&mysql);    

    std::cout << "Siema\n";
}
