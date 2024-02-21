#include "..\headers\authMiddleware.h"

void AuthMiddleware::before_handle(crow::request& req, crow::response& res, context& ctx)
{
	#if MIDDLEWARE_ACTIVATED
	bool authenticated = false;

	if (AUTH_NEEDED)
	{
		auto cookieHeader = req.get_header_value(SESSION_TOKEN_NAME);
		if (!cookieHeader.empty())
		{
			CROW_LOG_INFO << "Cookie header: " << cookieHeader;

			size_t pos = cookieHeader.find(SESSION_TOKEN_NAME) + SESSION_TOKEN_NAME.size() + 1;
			if (pos != std::string::npos)
			{
				std::string session_token = cookieHeader.substr(pos, SESSION_TOKEN_LENGTH);
				CROW_LOG_INFO << "Received session token: " << session_token;

				if (!database.getEmployeeBySessionToken(session_token).isEmpty()) authenticated = true;
			}
		}

		if (!authenticated && req.url != "/login")
		{
			res.redirect("/login");
			CROW_LOG_INFO << "Redirecting...";
		}
		else if (authenticated)
		{
			CROW_LOG_INFO << "Authenticated";
		}
	}
	#endif
}

void AuthMiddleware::after_handle(crow::request& req, crow::response& res, context& ctx)
{
}