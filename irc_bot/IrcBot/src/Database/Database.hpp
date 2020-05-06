#pragma once

#include <optional>

#include <mysql.h>

#include "Types.hpp"

#include "Model/Author.hpp"
#include "Model/Meme.hpp"

class Database
{
private:
	MYSQL _mysql;
	bool _autoReconnect = true;
	
public:
	Database();
	~Database();

	bool connect(const char* host, const char* user, const char* password, const char* database);

	Id addAuthor(const std::string& name);
	std::optional<Author> getAuthor(Id id);
	std::optional<Author> getAuthor(const std::string& name);

	Id addMeme(Id authorId, const std::string& memeUrl, const std::string& message);
	bool setKarma(Id memeId, int value);

	uint32_t getErrno();
	const char* getError();

	MYSQL& getMySQL() { return _mysql; }

private:
	void showErrorIfExist();
};
