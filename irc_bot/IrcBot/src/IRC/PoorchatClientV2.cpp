#include "PoorchatClientV2.hpp"

#ifdef WIN32
#include <WinSock2.h>
#endif

#include <loguru.hpp>
#include <libircclient.h>

#include "KarmaSystem.hpp"

PoorchatClientV2::PoorchatClientV2(KarmaSystem& karmaSystem)
	: _karmaSystem(karmaSystem)
{
}

PoorchatClientV2::~PoorchatClientV2()
{
	shutdown();
}

bool PoorchatClientV2::init()
{
	_ircCallbacks.event_connect = PoorchatClientV2::ircEventConnect;
	_ircCallbacks.event_channel = PoorchatClientV2::ircEventChannel;

	_ircSession = irc_create_session(&_ircCallbacks);
	irc_option_set(_ircSession, LIBIRC_OPTION_STRIPNICKS);


	if (!_ircSession)
	{
		LOG_F(ERROR, "Cannot create irc session!");
		return false;
	}

	irc_set_ctx(_ircSession, this);

	return true;
}

void PoorchatClientV2::shutdown()
{
	if (_ircSession != nullptr)
	{
		irc_destroy_session(_ircSession);
	}
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
	shutdown();
	
	if (!init())
	{
		LOG_F(INFO, "Cannot reinit IRC Client");
		return;
	}

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
	LOG_F(9, "%s: %s", userName, msg);

	if (strcmp(userName, "Pancernik") == 0)
	{
		return;
	}

	// find url
	{
		std::string url;

		if (re2::RE2::PartialMatch(msg, _url_re, &url))
		{
			_karmaSystem.addLink(userName, url);
		}
	}

	// find plus karma
	findKarmaAction(msg, userName, _karmaPlus_re, 1);
	findKarmaAction(msg, userName, _karmaMinus_re, -1);
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
		if (_maxErrorCount <= 0)
		{
			LOG_F(INFO, "Application is dead. Restart it manually");
			return;
		}

		LOG_F(INFO, "Could not connect or I/O error: %s", getError());

		reconnect();

		_maxErrorCount--;
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
