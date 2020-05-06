#pragma once

#include <string>

#include <re2/re2.h>
#include <libircclient.h>

class KarmaSystem;

class PoorchatClientV2
{
private:
	irc_callbacks_t _ircCallbacks = { 0 };
	irc_session_t* _ircSession = nullptr;

	KarmaSystem& _karmaSystem;

	std::string _ip;
	short _port;
	std::string _channelToJoin;

	re2::RE2 _url_re{ "\\b(([\\w-]+:\\/\\/?|www[.])[^\\s()<>]+(?:\\([\\w\\d]+\\)|([^[:punct:]\\s]|\\/)))" };
	re2::RE2 _karmaPlus_re{ "([a-zA-Z0-9_]+) *,* *\\+\\+" };
	re2::RE2 _karmaMinus_re{ "([a-zA-Z0-9_]+) *,* *\\-\\-" };

public:
	PoorchatClientV2(KarmaSystem& karmaSystem);
	~PoorchatClientV2();

	bool init();
	void shutdown();

	bool connect(const char* ip, short port, const char* channel);

	void run();

	const char* getError();

private:
	void joinChannel();
	void processMsg(const char* userName, const char* msg);
	void findKarmaAction(const std::string& message, const std::string& messageAuthor, const re2::RE2& regex, int value);

private:
	static void ircEventConnect(irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count);
	static void ircEventChannel(irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count);
};
