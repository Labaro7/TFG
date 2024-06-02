#pragma once
#ifndef _AUTHMIDDLEWARE_HPP_
#define _AUTHMIDDLEWARE_HPP_

#include "crow_all.hpp"
#include "constants.hpp"
#include "server.hpp"
#include "database.hpp"

class AuthMiddleware
{
public:
	AuthMiddleware(); // Needed for the crow::App<AuthMiddleware> construction

	struct context
	{
	};

	void before_handle(crow::request& req,
					   crow::response& res,
					   context& ctx);

	void after_handle(crow::request& req,
					  crow::response& res,
					  context& ctx);

private:
	Server& server;
};

#endif // !_AUTHMIDDLEWARE_HPP_

