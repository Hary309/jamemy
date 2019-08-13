#pragma once

#include <string>

#include "Database/Types.hpp"

#include "Author.hpp"

struct Meme
{
	int id;
	Author author;
	std::string url;
	int karma;
};
