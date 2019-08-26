#include "Database.hpp"

#include <string>

#include <loguru.hpp>

Database::Database()
{
	mysql_init(&_mysql);
}

Database::~Database()
{
	mysql_close(&_mysql);
}

bool Database::connect(const char* host, const char* user, const char* password, const char* database)
{
	if (mysql_real_connect(&_mysql, host, user, password, database, 0, nullptr, 0))
	{
		return true;
	}

	return false;
}

Id Database::addAuthor(const std::string& name)
{
	auto query = "INSERT INTO author VALUES(NULL, '" + name + "');";

	if (mysql_query(&_mysql, query.c_str()) == 0)
	{
		return mysql_insert_id(&_mysql);
	}
	
	return 0;
}

std::optional<Author> Database::getAuthor(Id id)
{
	auto query = "SELECT * FROM author WHERE id = " + std::to_string(id);

	if (mysql_query(&_mysql, query.c_str()) == 0)
	{
		auto res = mysql_store_result(&_mysql);

		MYSQL_ROW row = mysql_fetch_row(res);

		if (row == 0)
			return {};

		Author author;
		author.id = std::stoull(row[0]);
		author.name = row[1];
		return author;
	}

	return {};
}

std::optional<Author> Database::getAuthor(const std::string& name)
{
	auto query = "SELECT * FROM author WHERE name = '" + name + "'";

	if (mysql_query(&_mysql, query.c_str()) == 0)
	{
		auto res = mysql_store_result(&_mysql);

		MYSQL_ROW row = mysql_fetch_row(res);

		if (row == 0)
			return {};

		Author author;
		author.id = std::stoull(row[0]);
		author.name = row[1];
		return author;
	}

	return {};
}

Id Database::addMeme(Id authorId, const std::string& memeUrl, const std::string& message)
{
	auto query = "INSERT INTO meme VALUES (NULL, ?, ?, 0, NOW(), ?);";

	auto stmt = mysql_stmt_init(&_mysql);

	if (mysql_stmt_prepare(stmt, query, -1))
	{
		LOG_F(ERROR, "Error(%d) [%s] \"%s\"", mysql_stmt_errno(stmt),
			mysql_stmt_sqlstate(stmt),
			mysql_stmt_error(stmt));

		return 0;
	}

	MYSQL_BIND bind[3] = { 0 };

	bind[0].buffer_type = MYSQL_TYPE_LONG;
	bind[0].buffer = &authorId;

	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer = (void*)memeUrl.c_str();
	bind[1].buffer_length = memeUrl.size();

	bind[2].buffer_type = MYSQL_TYPE_STRING;
	bind[2].buffer = (void*)message.c_str();
	bind[2].buffer_length = message.size();

	const int arraySize = 1;
	mysql_stmt_bind_param(stmt, bind);

	if (mysql_stmt_execute(stmt))
	{
		LOG_F(ERROR, "Error(%d) [%s] \"%s\"", mysql_stmt_errno(stmt),
			mysql_stmt_sqlstate(stmt),
			mysql_stmt_error(stmt));

		return 0;
	}

	mysql_stmt_close(stmt);

	return mysql_insert_id(&_mysql);
}

bool Database::setKarma(Id memeId, int value)
{
	auto query = "UPDATE meme SET karma = " + std::to_string(value) + " WHERE id = " + std::to_string(memeId);

	if (mysql_query(&_mysql, query.c_str()) == 0)
	{
		return true;
	}

	return false;
}

uint32_t Database::getErrno()
{
	return mysql_errno(&_mysql);
}

const char* Database::getError()
{
	return mysql_error(&_mysql);
}
