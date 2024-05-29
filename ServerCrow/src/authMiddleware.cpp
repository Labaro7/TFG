#include "..\headers\authMiddleware.hpp"

void AuthMiddleware::before_handle(crow::request& req, crow::response& res, context& ctx)
{
	#if MIDDLEWARE_ACTIVATED
	bool authenticated = false;

	if (cts::AUTH_NEEDED)
	{
		auto cookieHeader = req.get_header_value(cts::SESSION_TOKEN_NAME);
		if (!cookieHeader.empty())
		{
			CROW_LOG_INFO << "Cookie header: " << cookieHeader;

			size_t pos = cookieHeader.find(cts::SESSION_TOKEN_NAME) + cts::SESSION_TOKEN_NAME.size() + 1;
			if (pos != std::string::npos)
			{
				std::string session_token = cookieHeader.substr(pos, cts::SESSION_TOKEN_LENGTH);
				CROW_LOG_INFO << "Received session token: " << session_token;

				if (!database.getEmployeeBySessionToken(session_token).isEmpty()) authenticated = true;
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