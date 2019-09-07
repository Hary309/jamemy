#include "SMTPClient.hpp"

#include <smtp.h>

#include <loguru.hpp>

SMTPClient::~SMTPClient()
{
	shutdown();
}

bool SMTPClient::init(const char* server, const char* port, const char* user, const char* password, const char* mailTo)
{
	int rc;

	rc = smtp_open(server, port, SMTP_SECURITY_NONE, SMTP_NO_CERT_VERIFY, nullptr, &_smtp);

	if (rc != SMTP_STATUS_OK)
	{
		LOG_F(ERROR, "Cannot open smtp!");
		return false;
	}

	rc = smtp_auth(_smtp, SMTP_AUTH_PLAIN, user, password);

	if (rc != SMTP_STATUS_OK)
	{
		LOG_F(ERROR, "Cannot auth smtp!");
		return false;
	}

	_mailTo = mailTo;
	_user = user;

	_isReady = true;

	return true;
}

void SMTPClient::shutdown()
{
	if (_smtp)
	{
		smtp_close(_smtp);
		_isReady = false;
	}
}

void SMTPClient::sendFault()
{
	smtp_address_add(_smtp, SMTP_ADDRESS_FROM, _user.c_str(), "IrcBot");
	smtp_address_add(_smtp, SMTP_ADDRESS_TO, _mailTo.c_str(), "Piotr Krupa");

	smtp_header_add(_smtp, "Subject", "Server is dead");

	auto result = smtp_mail(_smtp, ":(");

	if (result == SMTP_STATUS_OK)
	{
		LOG_F(INFO, "Email sent!");
	}
	else
	{
		LOG_F(INFO, "Cannot send mail! Code: %d", static_cast<int>(result));
	}
}
