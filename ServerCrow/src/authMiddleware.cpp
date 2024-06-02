#include "..\include\authMiddleware.hpp"

AuthMiddleware::AuthMiddleware() : server(Server::getInstance()) {}

void AuthMiddleware::before_handle(crow::request& req, crow::response& res, context& ctx)
{
	#if MIDDLEWARE_ACTIVATED
	bool authenticated = false;

	if (cts::AUTH_NEEDED)
	{
		const auto cookieHeader = req.get_header_value(cts::SESSION_TOKEN_NAME);
		if (!cookieHeader.empty())
		{
			CROW_LOG_INFO << "Cookie header: " << cookieHeader;

			const size_t pos = cookieHeader.find(cts::SESSION_TOKEN_NAME) + cts::SESSION_TOKEN_NAME.size() + 1;
			if (pos != std::string::npos)
			{
				const std::string session_token = cookieHeader.substr(pos, cts::SESSION_TOKEN_LENGTH);
				CROW_LOG_INFO << "Received session token: " << session_token;

				if (!server.db()->getEmployeeBySessionToken(session_token).isEmpty()) authenticated = true;
			}
		}

		if (!authenticated && req.url != "/login" && req.url != "/static/login.css" && req.url != "/static/login.js" && req.url != "/static/favicon.png")
		{
			req.url = "/login";

			CROW_LOG_INFO << "Redirecting...";
		}
		else
		{
			CROW_LOG_INFO << "Authenticated";
		}
	}
	#endif
}

void AuthMiddleware::after_handle(crow::request& req, crow::response& res, context& ctx)
{
}