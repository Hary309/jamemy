#include "PoorchatClientV2.hpp"

#include <WinSock2.h>

#include <loguru.hpp>
#include <libircclient.h>

#include "KarmaSystem.hpp"

PoorchatClientV2::PoorchatClientV2(KarmaSystem& karmaSystem)
	: _karmaSystem(karmaSystem)
{
}

bool PoorchatClientV2::init()
{
#ifdef WIN32
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		LOG_F(ERROR, "Unable to initialize Winsock.");
		return false;
	}
#endif

	_ircCallbacks.event_connect = PoorchatClientV2::ircEventConnect;
	_ircCallbacks.event_channel = PoorchatClientV2::ircEventChannel;

	_ircSession = irc_create_session(&_ircCallbacks);

	if (!_ircSession)
	{
		LOG_F(ERROR, "Cannot create irc session!");
		return false;
	}

	irc_set_ctx(_ircSession, this);

	return true;
}

bool PoorchatClientV2::connect(const char* ip, short port, const char* channel)
{
	_ip = ip;
	_port = port;
	_channelToJoin = channel;

	return irc_connect(_ircSession, ip, port, 0, "Bot", 0, 0) == 0;
}

void PoorchatClientV2::reconnect()
{
	LOG_F(INFO, "Reconnecting...");

	if (irc_connect(_ircSession, _ip.c_str(), _port, 0, "Bot", 0, 0) != 0)
	{
		LOG_F(ERROR, "Cannot reconnect (%s)", getError());
	}
}

void PoorchatClientV2::joinChannel()
{
	if (irc_cmd_join(_ircSession, _channelToJoin.c_str(), 0) != 0)
	{
		LOG_F(ERROR, "Cannot join channel!");
	}
}

void PoorchatClientV2::processMsg(const char* userName, const char* msg)
{
	std::string realUserName = userName;

	auto pos = realUserName.find('!');
	if (pos != std::string::npos)
	{
		realUserName = realUserName.substr(0, pos);
	}

	LOG_F(9, "%s: %s", realUserName.c_str(), msg);

	if (realUserName == "Pancernik")
	{
		return;
	}

	// find url
	{
		std::string url;

		if (re2::RE2::PartialMatch(msg, _url_re, &url))
		{
			_karmaSystem.addLink(realUserName, url);
		}
	}

	// find plus karma
	findKarmaAction(msg, realUserName, _karmaPlus_re, 1);
	findKarmaAction(msg, realUserName, _karmaMinus_re, -1);
}

void PoorchatClientV2::findKarmaAction(const std::string& message, const std::string& messageAuthor, const re2::RE2& regex, int value)
{
	std::string targetAuthorName;

	if (re2::RE2::PartialMatch(message, regex, &targetAuthorName))
	{
		if (messageAuthor != targetAuthorName)
		{
			LOG_SCOPE_F(INFO, "Karma");
			LOG_F(INFO, "%s gave %d karma to %s!", messageAuthor.c_str(), value, targetAuthorName.c_str());
			_karmaSystem.giveKarma(targetAuthorName, value);
		}
	}
}

void PoorchatClientV2::run()
{
	while (irc_run(_ircSession) != 0)
	{
		LOG_F(INFO, "Could not connect or I/O error:  %s", getError());

		reconnect();
	}
}

const char* PoorchatClientV2::getError()
{
	return irc_strerror(irc_errno(_ircSession));
}

void PoorchatClientV2::ircEventConnect(irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count)
{
	auto ctx = reinterpret_cast<PoorchatClientV2*>(irc_get_ctx(session));

	if (!irc_is_connected(session))
	{
		LOG_F(ERROR, "Not connected to server :(");
		return;
	}

	LOG_F(INFO, "Connected to server!");

	ctx->joinChannel();
}

void PoorchatClientV2::ircEventChannel(irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count)
{
	auto ctx = reinterpret_cast<PoorchatClientV2*>(irc_get_ctx(session));

	ctx->processMsg(origin, params[1]);
}
