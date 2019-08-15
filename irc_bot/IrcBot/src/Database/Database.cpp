#include "Database.hpp"

#include <string>

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

Id Database::addMeme(Id authorId, const std::string& memeUrl)
{
	auto query = "INSERT INTO meme VALUES (NULL, " + std::to_string(authorId) + ", '" + memeUrl + "', 0, NOW());";

	if (mysql_query(&_mysql, query.c_str()) == 0)
	{
		return mysql_insert_id(&_mysql);
	}

	return 0;
}

std::optional<Meme> Database::getMeme(Id memeId)
{
	auto query =
		"SELECT meme.id, author.id, author.name, meme.image_url, meme.karma "
		"FROM meme "
		"JOIN author ON meme.author_id = author.id "
		"WHERE meme.id = " + std::to_string(memeId);

	if (mysql_query(&_mysql, query.c_str()) == 0)
	{
		auto res = mysql_store_result(&_mysql);

		MYSQL_ROW row = mysql_fetch_row(res);

		if (row == 0)
			return {};

		Meme meme;
		meme.id = std::stoull(row[0]);
		meme.author.id = std::stoull(row[1]);
		meme.author.name = row[2];
		meme.url = row[3];
		meme.karma = std::stoi(row[4]);
		return meme;
	}

	return {};
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
