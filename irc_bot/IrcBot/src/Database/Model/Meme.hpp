#pragma once

#include <string>

#include "Database/Types.hpp"

#include "Author.hpp"

struct Meme
{
	Id id;
	Author author;
	std::string url;
	int karma;
};
