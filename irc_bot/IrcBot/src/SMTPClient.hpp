#pragma once

#include <string>

struct smtp;

class SMTPClient
{
private:
	smtp* _smtp = nullptr;
	std::string _mailTo;
	std::string _user;

	bool _isReady = false;

public:
	SMTPClient() = default;
	~SMTPClient();

	bool init(const char* server, const char* port, const char* user, const char* password, const char* mailTo);
	void shutdown();

	void sendFault();

	bool isReady() const { return _isReady; }
};
