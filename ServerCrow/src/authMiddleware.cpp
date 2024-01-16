#include "..\headers\authMiddleware.h"

void AuthMiddleware::before_handle(crow::request& req, crow::response& res, context& ctx) {
    bool authenticated = false;

    if (AUTH_NEEDED) {
        auto cookieHeader = req.get_header_value(SESSION_TOKEN_NAME);
        if (!cookieHeader.empty()) {
            std::cout << "cookieHeader: " << cookieHeader << std::endl;

            size_t pos = cookieHeader.find(SESSION_TOKEN_NAME) + SESSION_TOKEN_NAME.size() + 1;
            if (pos != std::string::npos) {
                std::string session_token = cookieHeader.substr(pos, 32);
                std::cout << "Session Token: " << session_token << std::endl;

                if (!database.getEmployeeBySessionToken(session_token).isEmpty()) authenticated = true;
            }
        }

        if (!authenticated && req.url != "/login") {
            res.redirect("https://192.168.1.66:18080/login");
            CROW_LOG_INFO << "Redirecting...";
        }
        else if (authenticated) {
            CROW_LOG_INFO << "Authenticated";
            //res.redirect(req.url);
        }
    }
}

void AuthMiddleware::after_handle(crow::request& req, crow::response& res, context& ctx) {
}