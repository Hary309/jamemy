#include <iostream>
#include <mysql.h>

int main()
{
    MYSQL mysql;
    mysql_init(&mysql);

    if (mysql_real_connect(&mysql, "127.0.0.1", "root", "", "jamemy", 0, nullptr, 0))
    {
        printf("Connected!\n");
    }
    else
    {
        printf("Cannot connect! %s\n", mysql_error(&mysql));
    }

    mysql_close(&mysql);    

    std::cout << "Siema\n";
    std::cin.get();
}
