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
	Database database;
};

#endif // !_AUTHMIDDLEWARE_HPP_

