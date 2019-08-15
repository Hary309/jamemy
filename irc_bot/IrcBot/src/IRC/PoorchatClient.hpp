#pragma once

#include <re2/re2.h>

#include "Socket.hpp"

class KarmaSystem;

class PoorchatClient
{
private:
	Socket _client;
	KarmaSystem& _karmaSystem;

	int _stage = 0;

	bool _joinedChannel = false;

	std::string _channelToJoin;

	re2::RE2 _privMsg_re{ ":(.*)!.* PRIVMSG .* :(.*)\r\n" };
	re2::RE2 _url_re{ "\\b(([\\w-]+:\\/\\/?|www[.])[^\\s()<>]+(?:\\([\\w\\d]+\\)|([^[:punct:]\\s]|\\/)))" };
	re2::RE2 _karmaPlus_re{ "([a-zA-Z0-9_]+) *,* *\\+\\+" };
	re2::RE2 _karmaMinus_re{ "([a-zA-Z0-9_]+) *,* *\\-\\-" };

public:
	PoorchatClient(KarmaSystem& karmaSystem);

	bool connect(const char* ip, short port, const char* channel);

	void update();

private:
	void findKarmaAction(const std::string& message, const std::string& messageAuthor, const re2::RE2& regex, int value);
};
